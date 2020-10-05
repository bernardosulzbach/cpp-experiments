#include "struct_reordering.hpp"

#include <iostream>

#include "types.hpp"

namespace Experiments {
struct A {
  U8 a;
  U32 b;
  U8 c;
};

struct B {
public:
  U8 a;

public:
  U32 b;

public:
  U8 c;
};

void testStructReordering() noexcept {
  std::cout << "This compiler did ";
  if (sizeof(A) == sizeof(B)) {
    std::cout << "not ";
  }
  std::cout << "reorder structs.\n";
}
} // namespace Experiments