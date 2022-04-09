#ifndef INSPECTOR_HANDLER_FONTS_HH
#define INSPECTOR_HANDLER_FONTS_HH

#include "../handler.hh"

namespace rlxos {
class FontHandler : public Handler {
 public:
  FontHandler(std::string path) : Handler(path) {}

  ~FontHandler() {}

  std::shared_ptr<Info> info();

  bool install();

  bool remove();
};
}  // namespace rlxos

#endif