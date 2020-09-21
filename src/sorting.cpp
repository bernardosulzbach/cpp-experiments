#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

#include "formatting.hpp"
#include "memory.hpp"

namespace Experiments {
static std::vector<std::mt19937::result_type> makeRandomIntegerVector(const std::size_t size) {
  std::vector<std::mt19937::result_type> integers;
  integers.reserve(size);
  std::mt19937 generator(0);
  for (std::size_t i = 0; i < size; i++) {
    integers.push_back(generator());
  }
  return integers;
}

template <typename SortFunction> static void testSortFunctionAllocations(const SortFunction &sortFunction, const std::string &name) {
  for (std::size_t size = 10; size <= 1'000'000u; size *= 10) {
    auto vector = makeRandomIntegerVector(size);
    std::cout << "Running " << name << " on " << size << " " << getPrettyTypeName<decltype(vector)::value_type>() << " elements.\n";
    AllocationTrackerGuard allocationTrackerGuard(true, false);
    sortFunction(std::begin(vector), std::end(vector));
  }
}

void testSortAllocations() {
  testSortFunctionAllocations([](auto begin, auto end) { std::sort(begin, end); }, "std::sort");
}

void testStableSortAllocations() {
  testSortFunctionAllocations([](auto begin, auto end) { std::stable_sort(begin, end); }, "std::stable_sort");
}
} // namespace Experiments