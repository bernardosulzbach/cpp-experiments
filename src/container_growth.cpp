#include "container_growth.hpp"

#include <iomanip>
#include <iostream>
#include <unordered_set>
#include <vector>

#include "formatting.hpp"
#include "memory.hpp"
#include "types.hpp"

namespace Experiments {
static constexpr U32 FactorDecimalPlaces = 4;
static constexpr std::size_t TargetSize = 1'000'000;
static constexpr std::size_t ReserveTargetSize = 50;
static constexpr U32 CapacityWidth = 8; // std::log10(10 * 10 * TargetSize)

template <typename T> void updateIfChangedAndNotify(T &lastT, const T newT) {
  if (lastT == newT) {
    return;
  }
  std::cout << Indentation << std::setw(CapacityWidth) << newT;
  if (lastT != 0) {
    const auto factor = newT / static_cast<double>(lastT);
    std::cout << " (x" << toFixedPrecisionString(factor, FactorDecimalPlaces) << ")";
  }
  std::cout << "\n";
  lastT = newT;
}

void testVectorMaximumSize() {
  std::cout << "std::vector<bool> maximum size: " << toStringWithThousandsSeparators(std::vector<bool>().max_size()) << "\n";
  std::cout << "std::vector<int> maximum size: " << toStringWithThousandsSeparators(std::vector<int>().max_size()) << "\n";
}

void testVectorGrowth() {
  std::vector<int> vector;
  auto lastCapacity = vector.capacity();
  std::cout << "Testing std::vector growth.\n";
  std::cout << Indentation << "It started with a capacity of " << lastCapacity << ".\n";
  for (std::size_t i = 0; i < TargetSize; i++) {
    vector.push_back(i);
    updateIfChangedAndNotify(lastCapacity, vector.capacity());
  }
}

void testVectorReserveGrowth() {
  std::vector<int> vector;
  auto lastCapacity = vector.capacity();
  std::cout << "Testing std::vector reserve() growth.\n";
  std::cout << Indentation << "It started with a capacity of " << lastCapacity << ".\n";
  AllocationTrackerGuard allocationTrackerGuard(false, false);
  for (std::size_t reserveSize = 1; reserveSize <= ReserveTargetSize; reserveSize++) {
    vector.reserve(reserveSize);
    updateIfChangedAndNotify(lastCapacity, vector.capacity());
  }
  if (allocationTrackerGuard.getAllocationsMade() >= ReserveTargetSize) {
    std::cout << Warning << "reserve() seems to reserve exactly the specified size.\n";
    std::cout << WarningIndentation
              << "This behavior will lead to quadratic performance if reserve() is regularly used before insertions of constant length.\n";
  }
}

void testUnorderedSetGrowth() {
  std::unordered_set<int> set;
  auto lastBucketCount = set.bucket_count();
  std::cout << "Testing std::unordered_set growth.\n";
  std::cout << Indentation << "It started with " << pluralizeAsNeeded(lastBucketCount, "bucket") << ".\n";
  std::cout << Indentation << "Its default maximum load factor is " << set.max_load_factor() << ".\n";
  for (std::size_t i = 0; i < TargetSize; i++) {
    set.insert(i);
    updateIfChangedAndNotify(lastBucketCount, set.bucket_count());
  }
}
} // namespace Experiments