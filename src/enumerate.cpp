#include "enumerate.hpp"

#include <string>
#include <vector>

namespace Experiments {
std::string enumerate(const std::vector<std::string> &strings) {
  std::string result;
  for (size_t i = 0; i < strings.size(); i++) {
    if (i > 0) {
      if (i + 1 < strings.size()) {
        result += ", ";
      } else {
        if (strings.size() == 2) {
          result += " and ";
        } else if (strings.size() > 2) {
          result += ", and ";
        }
      }
    }
    result += strings[i];
  }
  return result;
}
} // namespace Experiments