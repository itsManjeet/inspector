#include "inspector.hh"
using namespace rlxos;

#include <string.h>
#include <unistd.h>

#include <iostream>
#include <map>
#include <stdexcept>
#include <thread>
using namespace std;

#include <filesystem>
namespace fs = std::filesystem;

#include "handler.hh"

Inspector::Inspector() : mDelay(chrono::milliseconds(5000)) {}

Inspector::~Inspector() {}

void Inspector::add(char const* dir) { mPaths.push_back(dir); }

void Inspector::inspect() {
  this_thread::sleep_for(mDelay);

  auto iter = mStore.begin();
  while (iter != mStore.end()) {
    if (!fs::exists(iter->first)) {
      auto handler = Handler::from_path(iter->first);
      if (handler != nullptr) {
        try {
            handler->handle_delete();
          } catch (std::exception const& exc) {
            cerr << "Error! " << exc.what() << endl;
          }
      }

      iter = mStore.erase(iter);
    } else {
      iter++;
    }
  }

  for (auto const& p : mPaths) {
    for (auto& f : fs::directory_iterator(p)) {
      if (!f.is_regular_file()) continue;

      auto cur_status = fs::last_write_time(f);
      if (mStore.find(f.path().string()) == mStore.end()) {
        mStore[f.path().string()] = cur_status;

        auto handler = Handler::from_path(f.path().string());
        if (handler != nullptr) {
          try {
            handler->handle_create();
          } catch (std::exception const& exc) {
            cerr << "Error! " << exc.what() << endl;
          }
        }

      } else {
        if (mStore[f.path().string()] != cur_status) {
          mStore[f.path().string()] = cur_status;
          auto handler = Handler::from_path(f.path().string());
          if (handler != nullptr) {
            try {
              handler->handle_create();
            } catch (std::exception const& exc) {
              cerr << "Error! " << exc.what() << endl;
            }
          }
        }
      }
    }
  }
}

void Inspector::start() {
  while (true) {
    try {
      inspect();
    } catch (std::exception const& exc) {
      cerr << "Error! " << exc.what() << endl;
    }
  }
}