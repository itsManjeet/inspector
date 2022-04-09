#ifndef INSPECTOR_HANDLER_APPIMAGE_HH
#define INSPECTOR_HANDLER_APPIMAGE_HH

#include <appimage/appimage++.h>

#include "../handler.hh"

namespace rlxos {
class AppImageHandler : public Handler {
 public:
  AppImageHandler(std::string path);

  ~AppImageHandler();

  std::shared_ptr<Info> info();

  bool install();

  bool remove();
};
}  // namespace rlxos

#endif