#ifndef INSPECTOR_HANDLER_HH
#define INSPECTOR_HANDLER_HH

#include <libnotify/notify.h>

#include <memory>
#include <string>
#include <unordered_map>

#include "info.hh"

namespace rlxos {
/**
 * @brief Handler class handle the events raised by inspector to perform
 * specific tasks
 *
 */
class Handler {
 public:
  using ActionData = std::tuple<std::string, std::string,
                                void (*)(NotifyNotification* notify,
                                         char* action, gpointer* data),
                                void*>;

 protected:
  std::shared_ptr<Info> mInfo;
  std::string mPath;
  std::string mDataDir;

 public:
  Handler(std::string path);

  ~Handler();

  static std::shared_ptr<Handler> from_path(std::string path);

  std::string get_hash();

  std::string get_data_path();

  bool handle_create();

  bool handle_delete();

  virtual std::shared_ptr<Info> info() = 0;

  virtual bool install() = 0;

  virtual bool remove() = 0;

  void register_package();

  void unregister_package();

  bool is_registered();

  void notify(std::string mesg, std::vector<ActionData> const& actions = {});
};
}  // namespace rlxos

#endif