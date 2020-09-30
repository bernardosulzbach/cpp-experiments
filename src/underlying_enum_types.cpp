#include <iostream>

#include "formatting.hpp"

namespace Experiments {
void testUnscopedEnumsUnderlyingTypes() {
  std::cout << "Testing the underlying types of unscoped enums.\n";
  {
    enum Enum { Value };
    std::cout << Indentation;
    std::cout << "Underlying type of enum Enum { Value }: ";
    std::cout << getPrettyTypeName<std::underlying_type_t<Enum>>() << ".\n";
  }
  {
    enum Enum { Negative = -1, Positive = 1 };
    std::cout << Indentation;
    std::cout << "Underlying type of enum Enum { Negative = -1, Positive = 1 }: ";
    std::cout << getPrettyTypeName<std::underlying_type_t<Enum>>() << ".\n";
  }
  {
    enum Enum { Value = std::numeric_limits<uint64_t>::max() };
    std::cout << Indentation;
    std::cout << "Underlying type of enum Enum { Value = std::numeric_limits<uint64_t>::max() }: ";
    std::cout << getPrettyTypeName<std::underlying_type_t<Enum>>() << ".\n";
  }
  {
    enum Enum { Negative = std::numeric_limits<int64_t>::min(), Positive = std::numeric_limits<int64_t>::max() };
    std::cout << Indentation;
    std::cout << "Underlying type of enum Enum { Negative = std::numeric_limits<int64_t>::min(), Positive = std::numeric_limits<int64_t>::max() }: ";
    std::cout << getPrettyTypeName<std::underlying_type_t<Enum>>() << ".\n";
  }
  {
    enum Enum { Negative = std::numeric_limits<uint64_t>::min(), Positive = std::numeric_limits<uint64_t>::max() };
    std::cout << Indentation;
    std::cout << "Underlying type of enum Enum { Negative = std::numeric_limits<uint64_t>::min(), Positive = std::numeric_limits<uint64_t>::max() }: ";
    std::cout << getPrettyTypeName<std::underlying_type_t<Enum>>() << ".\n";
  }
  {
    enum Enum { Negative = -1, Positive = std::numeric_limits<uint64_t>::max() };
    std::cout << Indentation;
    std::cout << "Underlying type of enum Enum { Negative = -1, Positive = std::numeric_limits<uint64_t>::max() }: ";
    std::cout << getPrettyTypeName<std::underlying_type_t<Enum>>() << ".\n";
  }
}

void testScopedEnumsUnderlyingTypes() {
  std::cout << "Testing the underlying types of scoped enums.\n";
  {
    enum class Enum { Value };
    std::cout << Indentation << "Underlying type of enum class Enum { Value }: ";
    std::cout << getPrettyTypeName<std::underlying_type_t<Enum>>() << ".\n";
  }
  {
    enum class Enum { Negative = -1, Positive = 1 };
    std::cout << Indentation << "Underlying type of enum class Enum { Negative = -1, Positive = 1 }: ";
    std::cout << getPrettyTypeName<std::underlying_type_t<Enum>>() << ".\n";
  }
}

void testUnderlyingEnumTypes() {
  testUnscopedEnumsUnderlyingTypes();
  testScopedEnumsUnderlyingTypes();
}
} // namespace Experiments
