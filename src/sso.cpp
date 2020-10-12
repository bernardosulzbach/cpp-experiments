#include "sso.hpp"

#include "formatting.hpp"
#include "memory.hpp"

#include <cassert>
#include <string>

namespace Experiments {
[[nodiscard]] static bool subjectToSmallStringOptimization(std::size_t size) {
  AllocationTrackerGuard allocationTrackerGuard(false, false);
  std::string string(size, 'A');
  return allocationTrackerGuard.getAllocationsMade() == 0;
}

[[nodiscard]] static std::size_t findMaximumSmallStringOptimizationSize() {
  std::size_t a = 1u;
  const std::size_t b = (1u << 24u) + 1u;
  auto count = b - a;
  while (count > 0u) {
    const auto step = count / 2u;
    const auto m = a + step;
    if (subjectToSmallStringOptimization(m)) {
      a = m + 1;
      count -= step + 1u;
    } else {
      count = step;
    }
  }
  return a - 1u;
}

void testSmallStringOptimizationSize() {
  std::cout << "std::string takes " << pluralizeAsNeeded(sizeof(std::string), "byte") << ".\n";
  const auto maximumSmallStringOptimizationSize = findMaximumSmallStringOptimizationSize();
  if (maximumSmallStringOptimizationSize == 0) {
    std::cout << "No small string optimization (SSO) support.\n";
  }
  std::cout << "Small string optimization (SSO) support for strings with up to " << maximumSmallStringOptimizationSize;
  std::cout << " byte";
  if (maximumSmallStringOptimizationSize > 1) {
    std::cout << "s";
  }
  std::cout << ".\n";
}
} // namespace Experiments
