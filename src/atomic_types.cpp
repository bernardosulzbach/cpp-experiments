#include <atomic>
#include <iostream>

#include "formatting.hpp"

namespace Experiments {
void testSizesOfAtomicTypes() {
  std::cout << "Testing the sizes of some notable atomic types.\n";
  std::cout << Indentation << getPrettyTypeName<std::atomic_flag>() << "\n";
#if __cpp_lib_atomic_lock_free_type_aliases
  std::cout << Indentation << testSizeOfAtomicType(std::atomic_signed_lock_free) << "\n";
  std::cout << Indentation << testSizeOfAtomicType(std::atomic_unsigned_lock_free) << "\n";
#else
  std::cout << Indentation << "No std::atomic_signed_lock_free or std::atomic_unsigned_lock_free.\n";
#endif
}
} // namespace Experiments