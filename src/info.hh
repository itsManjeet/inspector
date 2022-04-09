#ifndef INSPECTOR_INFO_HH
#define INSPECTOR_INFO_HH

#include <string>
#include <vector>

namespace rlxos {
class Info {
 private:
  std::string mName, mVersion;
  std::vector<std::string> mFiles;
  std::string mPath;

 public:
  Info(std::string name, std::string version, std::string path,
       std::vector<std::string> const& files = {})
      : mName(name), mVersion(version), mPath(path), mFiles(files) {}

  Info(std::istream& is);

  Info(std::string s);

  std::string const& name() const { return mName; }
  std::string const& version() const { return mVersion; }

  std::string const& path() const { return mPath; }

  std::vector<std::string> const& files() const { return mFiles; }

  std::vector<std::string>& files() { return mFiles; }

  void dump(std::ostream& os) const;
};
}  // namespace rlxos

#endif