#include "appimage.hh"

using namespace rlxos;

#include <sys/stat.h>

#include <filesystem>
#include <fstream>
#include <iostream>
using namespace std;

AppImageHandler::AppImageHandler(string path) : Handler(path) {}

AppImageHandler::~AppImageHandler() {}

std::shared_ptr<Info> AppImageHandler::info() {
  return std::make_shared<Info>(mPath, "AppImage", mPath);
}

bool AppImageHandler::install() {
  auto appImage = appimage::core::AppImage(mPath);
  auto f = appImage.getFormat();
  if (f == appimage::core::AppImageFormat::INVALID) {
    notify("Invalid AppImage " + mPath + ", skipping");
  }

  auto integrator = appimage::desktop_integration::IntegrationManager();
  if (integrator.isARegisteredAppImage(mPath)) return;

  integrator.registerAppImage(appImage);
  integrator.generateThumbnails(appImage);
  system("update-desktop-database ~/.local/share/applications");

  return true;
}

bool AppImageHandler::remove() {
  auto integrator = appimage::desktop_integration::IntegrationManager();
  integrator.unregisterAppImage(mPath);
  integrator.removeThumbnails(mPath);
  system("update-desktop-database ~/.local/share/applications");
  return true;
}