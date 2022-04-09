#ifndef INSPECTOR_HANDLER_APP_HH
#define INSPECTOR_HANDLER_APP_HH

#include "../handler.hh"

namespace rlxos {
class AppHandler : public Handler {
 private:
  std::string mTempDir;

  std::string extract(std::string filepath);

  void patch_desktop(std::string src, std::string dest);

 public:
  AppHandler(std::string path);

  ~AppHandler();

  std::shared_ptr<Info> info();

  bool install();

  bool remove();
};
}  // namespace rlxos

#endif