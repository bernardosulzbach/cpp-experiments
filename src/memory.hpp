#pragma once

#include <atomic>
#include <iostream>
#include <memory>
#include <optional>

namespace Experiments {
class AllocationMessageEnabler {
public:
  AllocationMessageEnabler();

  ~AllocationMessageEnabler();
};

class DeallocationMessageEnabler {
public:
  DeallocationMessageEnabler();

  ~DeallocationMessageEnabler();
};

class AllocationTrackerGuard {
  std::size_t allocationCountAtStart;
  std::optional<AllocationMessageEnabler> optionalAllocationMessageEnabler;
  std::optional<DeallocationMessageEnabler> optionalDeallocationMessageEnabler;

public:
  explicit AllocationTrackerGuard(bool allocationMessages, bool deallocationMessages);

  [[nodiscard]] std::size_t getAllocationsMade() const noexcept;

  virtual ~AllocationTrackerGuard() noexcept;
};
} // namespace Experiments