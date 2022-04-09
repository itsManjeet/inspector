#ifndef INSPECTOR_HH
#define INSPECTOR_HH

#include <errno.h>
#include <sys/types.h>

#include <chrono>
#include <filesystem>
#include <map>
#include <string>
#include <vector>

namespace rlxos {
/**
 * @brief Inspector provide interface to inspect the changes on
 * specified directory
 *
 */
class Inspector {
 private:
  std::map<std::string, std::filesystem::file_time_type> mStore;
  std::vector<std::string> mPaths;

  using duration = std::chrono::duration<int, std::milli>;
  duration mDelay;

  enum class Status {
    ADDED,
    UPDATED,
    REMOVED,
  };

  void inspect();

 public:
  /**
   * @brief Construct a new Inspector object
   * initialize the inotify
   */
  Inspector();

  ~Inspector();

  /**
   * @brief append the directory for inspection
   *
   * @param dir absolute path of directory
   */
  void add(char const* dir);

  /**
   * @brief start the inspection
   *
   */
  void start();
};
}  // namespace rlxos

#endif