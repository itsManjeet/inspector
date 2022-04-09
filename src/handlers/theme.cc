#include "theme.hh"

#include <array>
#include <filesystem>
#include <iostream>
#include <stdexcept>
using namespace std;

using namespace rlxos;

std::shared_ptr<Info> ThemeHandler::info() {
  if (mInfo != nullptr) {
    return mInfo;
  }

  array<char, 128> buffer;
  string result;

  unique_ptr<FILE, decltype(&pclose)> pipe(
      popen(("tar -xaf " + mPath + " -O- ./info").c_str(), "r"), pclose);

  if (!pipe) {
    throw runtime_error("popen() failed");
  }

  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }

  return std::make_shared<Info>(result);
}

bool ThemeHandler::install() {
  string HOME = getenv("HOME");
  string destPath = HOME + "/.themes/" + mInfo->name() + "-" + get_hash();

  error_code error;
  filesystem::create_directories(destPath, error);
  if (error) {
    cout << "Failed to create required directory " + destPath + ", " +
                error.message()
         << endl;

    return false;
  }

  int status = system(
      ("tar --exclude='./info' -xaf " + mPath + " -C " + destPath).c_str());
  if (status != 0) {
    cout << "Failed to extract fonts " << endl;
    return false;
  }
  mInfo->files().push_back(destPath);
  return true;
}

bool ThemeHandler::remove() {
  string HOME = getenv("HOME");
  string destPath = HOME + "/.themes/" + mInfo->name() + "-" + get_hash();

  error_code error;
  filesystem::remove_all(destPath, error);
  if (error) {
    cout << "Failed to clean theme" << endl;
    return false;
  }

  return true;
}