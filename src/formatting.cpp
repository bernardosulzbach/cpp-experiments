#include "formatting.hpp"

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "types.hpp"

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

std::string toFixedPrecisionString(double value, U32 decimalPlaces) {
  std::stringstream stream;
  stream << std::fixed << std::setprecision(static_cast<int>(decimalPlaces)) << value;
  return stream.str();
}

std::string pluralizeAsNeeded(const U64 value, const std::string_view noun) {
  std::string result = std::to_string(value);
  result += " ";
  result += noun;
  if (value != 1) {
    result += "s";
  }
  return result;
}

class WithThousandsSeparators : public std::numpunct<char> {
protected:
  char do_thousands_sep() const override { return ','; }
  std::string do_grouping() const override { return "\03"; }
};

[[nodiscard]] std::string toStringWithThousandsSeparators(U64 value) {
  std::stringstream stream;
  stream.imbue(std::locale(std::locale::classic(), new WithThousandsSeparators));
  stream << value;
  return stream.str();
}
} // namespace Experiments