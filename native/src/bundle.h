#ifndef RNL_BUNDLE_H
#define RNL_BUNDLE_H
#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

std::string dirnameOf(const std::string& fname)
{
  size_t pos = fname.find_last_of("\\/");
  return (std::string::npos == pos)
      ? ""
      : fname.substr(0, pos);
}

namespace bundle {
  std::string content() {
    auto exe_path = std::filesystem::canonical("/proc/self/exe");
    auto build_dir = dirnameOf(exe_path.string());
    std::string bundle_path = build_dir + "/bundle.js";

    std::ifstream fin(bundle_path.c_str(), std::ifstream::binary);
    std::string contents = R"(
      console.log = print; console.warn = print; console.error = print;
      function setTimeout(callback, ms) {
        print("ST: " + callback);
        return _setTimeout(callback, ms)
      }
      function clearTimeout(identifier) {
        print("CT: " + identifier);
        _clearTimeout(identifier)
      }
      function setImmediate(callback) {
        print("SI: " + callback);
        callback()
        // return _setImmediate(callback)
      }
    )";
    std::string line;

    while(std::getline(fin, line)) {
      contents += line + "\n";
    }

    return contents;
  }
};
#endif
