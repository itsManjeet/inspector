#include "info.hh"

#include <yaml-cpp/yaml.h>

using namespace rlxos;

#include <iostream>
using namespace std;

Info::Info(std::istream& is)
    : Info(string((istreambuf_iterator<char>(is)),
                  (istreambuf_iterator<char>()))) {}

Info::Info(std::string is) {
  YAML::Node node = YAML::Load(is);

  mName = node["id"].as<string>();
  mVersion = node["version"].as<string>();

  if (node["files"]) {
    for (auto const& i : node["files"]) {
      mFiles.push_back(i.as<string>());
    }
  }
}

void Info::dump(std::ostream& os) const {
  os << "id: " << name() << '\n' << "version: " << version() << endl;

  if (files().size()) {
    os << "files: \n";
    for (auto const& i : files()) {
      os << "  - " << i << '\n';
    }
  }
  os << flush;
}