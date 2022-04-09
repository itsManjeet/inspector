#ifndef INSPECTOR_HANDLER_THEME_HH
#define INSPECTOR_HANDLER_THEME_HH

#include "../handler.hh"

namespace rlxos {
class ThemeHandler : public Handler {
 public:
  ThemeHandler(std::string path) : Handler(path) {}

  ~ThemeHandler() {}

  std::shared_ptr<Info> info();

  bool install();

  bool remove();
};
}  // namespace rlxos

#endif