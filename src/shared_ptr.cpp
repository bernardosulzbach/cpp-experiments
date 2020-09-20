#include "shared_ptr.hpp"

#include <memory>

#include "memory.hpp"
#include "types.hpp"

void Experiments::testSharedPointerMemoryAllocations() {
  AllocationTrackerGuard allocationTrackerGuard(true, false);
  {
    std::cout << "Creating a shared pointer to an 8-byte integer with std::make_shared.\n";
    const auto sharedPtr = std::make_shared<U64>();
  }
  {
    std::cout << "Creating a shared pointer to an 8-byte integer after a manual allocation.\n";
    const auto sharedPtr = std::shared_ptr<U64>{new U64};
  }
  {
    std::cout << "Creating a shared pointer to a 128-byte array with std::make_shared.\n";
    const auto sharedPtr = std::make_shared<std::array<U8, 128>>();
  }
  {
    std::cout << "Creating a shared pointer to a 128-byte array after a manual allocation.\n";
    const auto sharedPtr = std::shared_ptr<std::array<U8, 128>>{new std::array<U8, 128>};
  }
}
