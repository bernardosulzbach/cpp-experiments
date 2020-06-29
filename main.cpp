#include <atomic>
#include <bitset>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

using U8 = uint8_t;

static std::atomic<bool> writingAllocationMessages = false;
static std::atomic<std::size_t> allocationCount = 0;

void *operator new(const std::size_t size) {
  if (allocationCount == std::numeric_limits<decltype(allocationCount)::value_type>::max()) {
    throw std::bad_alloc();
  }
  if (writingAllocationMessages) {
    std::cout << "Made an allocation of size " << size << ".\n";
  }
  allocationCount++;
  return std::malloc(size);
}

void operator delete(void *pointer) noexcept { std::free(pointer); }
void operator delete(void *pointer, const std::size_t size) noexcept { std::free(pointer); }

void disableAllocationMessages() { writingAllocationMessages = false; }

void enableAllocationMessages() { writingAllocationMessages = true; }

class AllocationTrackerGuard {
  std::size_t allocationCountAtStart;

public:
  AllocationTrackerGuard() : allocationCountAtStart(allocationCount) { enableAllocationMessages(); }

  virtual ~AllocationTrackerGuard() {
    if (allocationCount == allocationCountAtStart) {
      std::cout << "Made no allocations.\n";
    }
    disableAllocationMessages();
  }
};

void testVectorAssignment() {
  std::array<std::size_t, 3> sizes = {10, 100, 1000};
  for (const auto lhsSize : sizes) {
    for (const auto rhsSize : sizes) {
      std::vector<U8> lhs(lhsSize);
      std::vector<U8> rhs(rhsSize);
      std::cout << "Testing assigning vector of size " << rhsSize << " to a vector of size " << lhsSize << ". ";
      AllocationTrackerGuard allocationTrackerGuard;
      lhs = rhs;
    }
  }
}

int main() {
  testVectorAssignment();
  return 0;
}
