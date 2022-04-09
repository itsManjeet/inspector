#include "appimage.hh"

using namespace rlxos;

#include <sys/stat.h>

#include <filesystem>
#include <fstream>
#include <iostream>
using namespace std;

AppImageHandler::AppImageHandler(string path) : Handler(path) {
  mTempDir = "/tmp/" + get_hash();
  std::filesystem::create_directories(mTempDir);
}

std::string AppImageHandler::extract(string path) {
  string appImage = mPath;
  if (chmod(appImage.c_str(), 0755) != 0) {
    notify("Failed to set executable permission for " + appImage);
    return "";
  }

  if (system(
          ("cd " + mTempDir + " && " + appImage + " --appimage-extract " + path)
              .c_str()) != 0) {
    notify("Failed to extract '" + path + "' from appImage");
    return "";
  }

  return mTempDir + "/squashfs-root/" + path;
}

AppImageHandler::~AppImageHandler() {
  if (std::filesystem::exists(mTempDir)) std::filesystem::remove_all(mTempDir);
}

std::shared_ptr<Info> AppImageHandler::info() {
  if (mInfo != nullptr) {
    return mInfo;
  }

  string infoPath = extract("info");
  std::ifstream file(infoPath);
  if (!file.good()) {
    notify("Failed to read information from appimage");
    return nullptr;
  }
  mInfo = std::make_shared<Info>(file);
  return mInfo;
}

void AppImageHandler::patch_desktop(string src, string dest) {
  string data;

  ifstream srcFile(src);
  data = string((istreambuf_iterator<char>(srcFile)),
                (istreambuf_iterator<char>()));
  srcFile.close();

  std::string destIcon =
      string(getenv("HOME")) + "/.local/share/pixmaps/" + get_hash() + ".png";

  std::stringstream ss(data);
  std::string line;
  std::ofstream destFile(dest);
  while (std::getline(ss, line)) {
    if (!line.size()) {
      continue;
    }

    if (line.find("Exec=", 0) == 0) {
      auto idx = line.find_first_of('=');
      auto space = line.find_first_of(' ');

      auto newline = line.substr(0, idx) + "=" + mPath;
      if (space != std::string::npos) {
        newline += line.substr(space, line.length() - space);
      }
      line = newline;
    } else if (line.find("Icon=", 0) == 0) {
      line = "Icon=" + destIcon;
    } else if (line.find("TryExec=", 0) == 0) {
      continue;
    }
    destFile << line << std::endl;
  }
  destFile.close();
}

bool AppImageHandler::install() {
  std::string srcIcon = extract(mInfo->name() + ".png");
  std::string destIcon = string(getenv("HOME")) + "/.local/share/pixmaps";
  if (!std::filesystem::exists(destIcon)) {
    std::filesystem::create_directories(destIcon);
  }
  destIcon += "/" + get_hash() + ".png";
  std::filesystem::copy_file(srcIcon, destIcon);

  mInfo->files().push_back(destIcon);

  std::string srcDesktop = extract(mInfo->name() + ".desktop");
  std::string destDesktopDir =
      string(getenv("HOME")) + "/.local/share/applications";
  if (!std::filesystem::exists(destDesktopDir)) {
    std::filesystem::create_directories(destDesktopDir);
  }
  string destDesktop = destDesktopDir + "/" + get_hash() + ".desktop";
  patch_desktop(srcDesktop, destDesktop);
  mInfo->files().push_back(destDesktop);

  system(("update-desktop-database " + destDesktopDir).c_str());
  return true;
}

bool AppImageHandler::remove() {
  for (auto const& i : mInfo->files()) {
    std::filesystem::remove(i);
  }

  return true;
}