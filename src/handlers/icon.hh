#ifndef INSPECTOR_HANDLER_ICON_HH
#define INSPECTOR_HANDLER_ICON_HH

#include "../handler.hh"

namespace rlxos {
class IconHandler : public Handler {
 public:
  IconHandler(std::string path) : Handler(path) {}

  ~IconHandler() {}

  std::shared_ptr<Info> info();

  bool install();

  bool remove();
};
}  // namespace rlxos

#endif