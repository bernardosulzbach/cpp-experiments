#include "memory.hpp"

#include <atomic>
#include <limits>

static std::atomic<bool> writingAllocationMessages = false;
static std::atomic<bool> writingDeallocationMessages = false;

static std::atomic<std::size_t> allocationCount = 0;

void *operator new(const std::size_t size) {
  // This should be using "std::numeric_limits<decltype(allocationCount)::value_type>::max()", but LLVM does not have value_type for atomic.
  if (allocationCount == std::numeric_limits<std::size_t>::max()) {
    throw std::bad_alloc();
  }
  void *result = std::malloc(size);
  if (result == nullptr) {
    throw std::bad_alloc();
  }
  if (writingAllocationMessages) {
    std::cout << "Made an allocation of size " << size << ".\n";
  }
  allocationCount++;
  return result;
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
AllocationMessageEnabler::AllocationMessageEnabler() {
  if (writingAllocationMessages) {
    throw std::runtime_error("Tried to enable allocation messages a second time, this is not allowed.");
  }
  writingAllocationMessages = true;
}

AllocationMessageEnabler::~AllocationMessageEnabler() { writingAllocationMessages = false; }

DeallocationMessageEnabler::DeallocationMessageEnabler() {
  if (writingDeallocationMessages) {
    throw std::runtime_error("Tried to enable deallocation messages a second time, this is not allowed.");
  }
  writingDeallocationMessages = true;
}

DeallocationMessageEnabler::~DeallocationMessageEnabler() { writingDeallocationMessages = false; }

AllocationTrackerGuard::AllocationTrackerGuard(bool allocationMessages, bool deallocationMessages)
    : allocationCountAtStart(allocationCount),
      optionalAllocationMessageEnabler(allocationMessages ? std::make_optional<AllocationMessageEnabler>() : std::nullopt),
      optionalDeallocationMessageEnabler(deallocationMessages ? std::make_optional<DeallocationMessageEnabler>() : std::nullopt) {}

std::size_t AllocationTrackerGuard::getAllocationsMade() const noexcept { return allocationCount - allocationCountAtStart; }

AllocationTrackerGuard::~AllocationTrackerGuard() noexcept {
  if (optionalAllocationMessageEnabler && getAllocationsMade() == 0) {
    std::cout << "Made no allocations.\n";
  }
}
} // namespace Experiments
