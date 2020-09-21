#pragma once

#include <string>
#include <vector>

#include <boost/type_index.hpp>

namespace Experiments {
template <typename T> std::string getPrettyTypeName() {
  std::string name = boost::typeindex::type_id<T>().pretty_name();
  name += " (";
  name += std::to_string(8U * sizeof(T));
  name += " bits)";
  return name;
}

[[nodiscard]] std::string enumerate(const std::vector<std::string> &strings);
} // namespace Experiments
