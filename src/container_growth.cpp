#include "container_growth.hpp"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <unordered_set>
#include <vector>

#include "formatting.hpp"
#include "types.hpp"

namespace Experiments {
static constexpr U32 FactorDecimalPlaces = 4;
static constexpr std::size_t TargetSize = 1'000'000;
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