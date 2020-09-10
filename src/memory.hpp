#pragma once

#include <atomic>
#include <iostream>
#include <memory>

extern std::atomic<bool> writingAllocationMessages;
extern std::atomic<bool> writingDeallocationMessages;

extern std::atomic<std::size_t> allocationCount;

void disableAllocationMessages() noexcept;

void enableAllocationMessages();

void disableDeallocationMessages() noexcept;

void enableDeallocationMessages();

void *operator new(const std::size_t size);

void operator delete(void *pointer) noexcept;

void operator delete(void *pointer, const std::size_t size) noexcept;

namespace Experiments {
class AllocationTrackerGuard {
  std::size_t allocationCountAtStart;
  bool emittingAllocationMessages;

public:
  explicit AllocationTrackerGuard(bool allocationMessages, bool deallocationMessages);

  [[nodiscard]] std::size_t getAllocationsMade() const noexcept;

  virtual ~AllocationTrackerGuard() noexcept;
};
} // namespace Experiments