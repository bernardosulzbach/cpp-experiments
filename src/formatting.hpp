#pragma once

#include <string>
#include <string_view>
#include <vector>

#include <boost/type_index.hpp>

#include "types.hpp"

namespace Experiments {
static constexpr std::string_view Warning = "WARNING: ";
static constexpr std::string_view Indentation = "  ";

template <typename T> std::string getPrettyTypeName() {
  std::string name = boost::typeindex::type_id<T>().pretty_name();
  name += " (";
  name += std::to_string(8U * sizeof(T));
  name += " bits)";
  return name;
}

[[nodiscard]] std::string enumerate(const std::vector<std::string> &strings);

[[nodiscard]] std::string toFixedPrecisionString(double value, unsigned decimalPlaces);

[[nodiscard]] std::string pluralizeAsNeeded(U64 value, std::string_view noun);
} // namespace Experiments
