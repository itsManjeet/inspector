#ifndef INSPECTOR_HANDLER_APPIMAGE_HH
#define INSPECTOR_HANDLER_APPIMAGE_HH

#include "../handler.hh"

namespace rlxos {
class AppImageHandler : public Handler {
 private:
  std::string mTempDir;

  std::string extract(std::string filepath);

  void patch_desktop(std::string src, std::string dest);

 public:
  AppImageHandler(std::string path);

  ~AppImageHandler();

  std::shared_ptr<Info> info();

  bool install();

  bool remove();
};
}  // namespace rlxos

#endif