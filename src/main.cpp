#include <atomic>
#include <bitset>
#include <chrono>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "atomic_types.hpp"
#include "container_growth.hpp"
#include "experiment_runner.hpp"
#include "memory.hpp"
#include "shared_ptr.hpp"
#include "sorting.hpp"
#include "special_member_function_monitor.hpp"
#include "sso.hpp"
#include "struct_reordering.hpp"
#include "types.hpp"
#include "underlying_enum_types.hpp"

namespace Experiments {
void testVectorAssignment() {
  std::array<std::size_t, 3> sizes = {10, 100, 1000};
  for (const auto lhsSize : sizes) {
    for (const auto rhsSize : sizes) {
      std::vector<U8> lhs(lhsSize);
      std::vector<U8> rhs(rhsSize);
      std::cout << "Testing assigning vector of size " << rhsSize << " to a vector of size " << lhsSize << ". ";
      AllocationTrackerGuard allocationTrackerGuard(true, false);
      lhs = rhs;
    }
  }
}

void testVectorAllocationsAndFreesWithBlocks() {
  AllocationTrackerGuard allocationTrackerGuard(true, true);
  const std::size_t vectorSize = 4;
  std::cout << "Creating two vectors of size " << vectorSize << " in the same block.\n";
  {
    std::vector<std::byte> vectorA(vectorSize);
    std::vector<std::byte> vectorB(vectorSize);
  }
  std::cout << "Creating two vectors of size " << vectorSize << " in two consecutive blocks.\n";
  {
    { std::vector<std::byte> vector(vectorSize); }
    { std::vector<std::byte> vector(vectorSize); }
  }
}

void testInsertWithConflictingKeyInUnorderedMap() {
  std::unordered_map<int, int> map;
  map.insert(std::pair<int, int>{1, 2});
  map.insert(std::pair<int, int>{2, 3});
  map.insert(std::pair<int, int>{1, 3});
  for (const auto [key, value] : map) {
    std::cout << key << ": " << value << '\n';
  }
}

void testPushBackAndEmplaceBackAllocations() {
  const auto printSpecialMemberFunctionCallCount = [](const std::string &situation) {
    std::cout << "When using " << situation << ", ";
    SpecialMemberFunctionMonitor::print();
    std::cout << "\n";
    SpecialMemberFunctionMonitor::reset();
  };
  {
    std::vector<SpecialMemberFunctionMonitor> vector;
    SpecialMemberFunctionMonitor monitor;
    vector.push_back(monitor);
  }
  printSpecialMemberFunctionCallCount("push_back() with lvalue");
  {
    std::vector<SpecialMemberFunctionMonitor> vector;
    vector.push_back({});
  }
  printSpecialMemberFunctionCallCount("push_back() with rvalue");
  {
    std::vector<SpecialMemberFunctionMonitor> vector;
    SpecialMemberFunctionMonitor monitor;
    vector.emplace_back(monitor);
  }
  printSpecialMemberFunctionCallCount("emplace_back() with lvalue");
  {
    std::vector<SpecialMemberFunctionMonitor> vector;
    vector.emplace_back();
  }
  printSpecialMemberFunctionCallCount("emplace_back() constructor arguments");
}

static constexpr auto CPlusPlus98 = 199711L;
static constexpr auto CPlusPlus11 = 201103L;
static constexpr auto CPlusPlus14 = 201402L;
static constexpr auto CPlusPlus17 = 201703L;

void printStandard() noexcept {
  std::cout << "The C++ standard used ";
  if (__cplusplus < CPlusPlus98) {
    std::cout << "predates C++98";
  } else if (__cplusplus == CPlusPlus98) {
    std::cout << "is C++98";
  } else if (__cplusplus < CPlusPlus11) {
    std::cout << "predates C++11";
  } else if (__cplusplus == CPlusPlus11) {
    std::cout << "is C++11";
  } else if (__cplusplus < CPlusPlus14) {
    std::cout << "predates C++14";
  } else if (__cplusplus == CPlusPlus14) {
    std::cout << "is C++14";
  } else if (__cplusplus < CPlusPlus17) {
    std::cout << "predates C++17";
  } else if (__cplusplus == CPlusPlus17) {
    std::cout << "is C++17";
  } else {
    std::cout << "succeeds C++17";
  }
  std::cout << ".\n";
}

[[nodiscard]] int main() {
  printStandard();
  ExperimentRunner(testVectorAssignment).run();
  ExperimentRunner(testVectorAllocationsAndFreesWithBlocks).run();
  ExperimentRunner(testVectorMaximumSize).run();
  ExperimentRunner(testVectorGrowth).run();
  ExperimentRunner(testVectorReserveGrowth).run();
  ExperimentRunner(testUnorderedSetGrowth).run();
  ExperimentRunner(testInsertWithConflictingKeyInUnorderedMap).run();
  ExperimentRunner(testStringSize).run();
  ExperimentRunner(testStringMaximumSize).run();
  ExperimentRunner(testSmallStringOptimizationSize).run();
  ExperimentRunner(testUnderlyingEnumTypes).run();
  ExperimentRunner(testPushBackAndEmplaceBackAllocations).run();
  ExperimentRunner(testSharedPointerMemoryAllocations).run();
  ExperimentRunner(testSortAllocations).run();
  ExperimentRunner(testStableSortAllocations).run();
  ExperimentRunner(testSizesOfAtomicTypes).run();
  ExperimentRunner(testStructReordering).run();
  return EXIT_SUCCESS;
}
} // namespace Experiments

int main() { return Experiments::main(); }
