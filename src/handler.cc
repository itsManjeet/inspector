#include "handler.hh"
using namespace rlxos;

#include <libnotify/notify.h>

#include <filesystem>
#include <fstream>
#include <iostream>
using namespace std;

#include "handlers/appimage.hh"
#include "handlers/font.hh"
#include "handlers/icon.hh"
#include "handlers/theme.hh"

Handler::Handler(string path) : mPath(path) {
  const string HOME = getenv("HOME");
  mDataDir = HOME + "/.config/inspector/";
  if (!std::filesystem::exists(mDataDir)) {
    std::error_code err;
    std::filesystem::create_directories(mDataDir, err);
    if (err) {
      throw std::runtime_error("Failed to create required data dir, '" +
                               mDataDir + "', error: " + err.message());
    }
  }
}

Handler::~Handler() {}

std::shared_ptr<Handler> Handler::from_path(string path) {
  auto idx = path.find_last_of('.');
  if (idx == string::npos) {
    return nullptr;
  }

  auto ext = path.substr(idx + 1, path.length() - (idx + 1));

  if (ext == "app") {
    return std::make_shared<AppImageHandler>(path);
  } else if (ext == "theme") {
    return std::make_shared<ThemeHandler>(path);
  } else if (ext == "font") {
    return std::make_shared<FontHandler>(path);
  } else if (ext == "icon") {
    return std::make_shared<IconHandler>(path);
  } else {
    return nullptr;
  }
}

bool Handler::handle_create() {
  mInfo = info();
  if (mInfo == nullptr) {
    throw std::runtime_error("Failed to get information from package " + mPath);
  }
  if (is_registered()) {
    notify(mInfo->name() + "-" + mInfo->version() +
           " is already integrated into your system, skipping");
    return true;
  }

  notify("Integrating " + mInfo->name() + "-" + mInfo->version() +
         " into your system");

  if (!install()) {
    notify("Failed to integrate " + mInfo->name() +
           " into system, check log for more information");
    return false;
  }

  register_package();

  notify(mInfo->name() + " is integrated successfully");
  return true;
}

bool Handler::handle_delete() {
  auto data_path = get_data_path();
  if (!std::filesystem::exists(data_path)) {
    throw std::runtime_error("Failed to get information from package " + mPath +
                             ", at " + data_path);
  }

  {
    std::ifstream file(data_path);
    mInfo = std::make_shared<Info>(file);
  }

  if (!is_registered()) {
    notify(mInfo->name() + "-" + mInfo->version() +
           " is not already integrated into your system, skipping");
    return true;
  }

  notify("removing " + mInfo->name() + "-" + mInfo->version());
  if (!remove()) {
    return false;
  }

  unregister_package();
  return true;
}

void Handler::register_package() {
  std::ofstream file(get_data_path());
  mInfo->dump(file);
}

void Handler::unregister_package() { std::filesystem::remove(get_data_path()); }

bool Handler::is_registered() {
  return std::filesystem::exists(get_data_path());
}

void Handler::notify(std::string mesg, std::vector<ActionData> const& actions) {
  NotifyNotification* n;
  GError* error = nullptr;

  n = notify_notification_new("Inspector", mesg.c_str(), "software");
  if (n == nullptr) {
    return;
  }
  for (auto const& i : actions) {
    notify_notification_add_action(n, get<0>(i).c_str(), get<1>(i).c_str(),
                                   NOTIFY_ACTION_CALLBACK(get<2>(i)), get<3>(i),
                                   nullptr);
  }
  notify_notification_show(n, nullptr);
}

std::string Handler::get_hash() {
  hash<string> hasher;
  return to_string(hasher(mPath));
}

string Handler::get_data_path() { return mDataDir + "/" + get_hash(); }