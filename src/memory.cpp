#include "memory.hpp"

#include <atomic>
#include <limits>

std::atomic<bool> writingAllocationMessages = false;
std::atomic<bool> writingDeallocationMessages = false;

std::atomic<std::size_t> allocationCount = 0;

void disableAllocationMessages() noexcept { writingAllocationMessages = false; }

void enableAllocationMessages() {
  if (writingAllocationMessages) {
    throw std::runtime_error("Tried to enable allocation messages a second time, this is not allowed.");
  }
  writingAllocationMessages = true;
}

void disableDeallocationMessages() noexcept { writingDeallocationMessages = false; }

void enableDeallocationMessages() {
  if (writingDeallocationMessages) {
    throw std::runtime_error("Tried to enable deallocation messages a second time, this is not allowed.");
  }
  writingDeallocationMessages = true;
}

void *operator new(const std::size_t size) {
  // This should be using "std::numeric_limits<decltype(allocationCount)::value_type>::max()", but LLVM does not have value_type for atomic.
  if (allocationCount == std::numeric_limits<std::size_t>::max()) {
    throw std::bad_alloc();
  }
  if (writingAllocationMessages) {
    std::cout << "Made an allocation of size " << size << ".\n";
  }
  allocationCount++;
  return std::malloc(size);
}

void *operator new(const std::size_t size, const std::nothrow_t &) noexcept {
  try {
    return operator new(size);
  } catch (const std::exception &exception) {
    return nullptr;
  }
}

void operator delete(void *pointer) noexcept {
  if (writingDeallocationMessages) {
    std::cout << "Freed a pointer.\n";
  }
  std::free(pointer);
}

void operator delete(void *pointer, const std::size_t size) noexcept {
  if (writingDeallocationMessages) {
    std::cout << "Freed an array of size " << size << ".\n";
  }
  std::free(pointer);
}

namespace Experiments {
AllocationTrackerGuard::AllocationTrackerGuard(bool allocationMessages, bool deallocationMessages)
    : allocationCountAtStart(allocationCount), emittingAllocationMessages(allocationMessages) {
  if (allocationMessages) {
    enableAllocationMessages();
  }
  if (deallocationMessages) {
    enableDeallocationMessages();
  }
}

std::size_t AllocationTrackerGuard::getAllocationsMade() const noexcept { return allocationCount - allocationCountAtStart; }

AllocationTrackerGuard::~AllocationTrackerGuard() noexcept {
  if (emittingAllocationMessages && getAllocationsMade() == 0) {
    std::cout << "Made no allocations.\n";
  }
  disableAllocationMessages();
  disableDeallocationMessages();
}
} // namespace Experiments
