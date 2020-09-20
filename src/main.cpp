#include <atomic>
#include <bitset>
#include <chrono>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <boost/type_index.hpp>

#include "memory.hpp"
#include "shared_ptr.hpp"
#include "special_member_function_monitor.hpp"
#include "sso.hpp"
#include "types.hpp"

namespace Experiments {
template <typename T> std::string getPrettyTypeName() {
  std::string name = boost::typeindex::type_id<T>().pretty_name();
  name += " (";
  name += std::to_string(8U * sizeof(T));
  name += " bits)";
  return name;
}

template <typename T> bool hasChanged(T &lastT, const T newT) {
  if (lastT == newT) {
    return false;
  }
  lastT = newT;
  return true;
}

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

void testUnorderedSetGrowth() {
  std::unordered_set<int> set;
  size_t lastBucketCount = 0;
  for (int i = 0; i < 100'000; i++) {
    set.insert(i);
    if (hasChanged(lastBucketCount, set.bucket_count())) {
      std::cout << "Bucket count changed to " << lastBucketCount << ".\n";
    }
  }
}

void testUnscopedEnumsUnderlyingTypes() {
  std::cout << "Testing the underlying types of unscoped enums.\n";
  {
    enum Enum { Value };
    std::cout << "Underlying type of enum Enum { Value }: ";
    using UnderlyingType = std::underlying_type_t<Enum>;
    std::cout << getPrettyTypeName<UnderlyingType>() << ".\n";
  }
  {
    enum Enum { Negative = -1, Positive = 1 };
    std::cout << "Underlying type of enum Enum { Negative = -1, Positive = 1 }: ";
    using UnderlyingType = std::underlying_type_t<Enum>;
    std::cout << getPrettyTypeName<UnderlyingType>() << ".\n";
  }
  {
    enum Enum { Value = std::numeric_limits<uint64_t>::max() };
    std::cout << "Underlying type of enum Enum { Value = std::numeric_limits<uint64_t>::max() }: ";
    using UnderlyingType = std::underlying_type_t<Enum>;
    std::cout << getPrettyTypeName<UnderlyingType>() << ".\n";
  }
  {
    enum Enum { Negative = std::numeric_limits<int64_t>::min(), Positive = std::numeric_limits<int64_t>::max() };
    std::cout << "Underlying type of enum Enum { Negative = std::numeric_limits<int64_t>::min(), Positive = std::numeric_limits<int64_t>::max() }: ";
    using UnderlyingType = std::underlying_type_t<Enum>;
    std::cout << getPrettyTypeName<UnderlyingType>() << ".\n";
  }
  {
    enum Enum { Negative = std::numeric_limits<uint64_t>::min(), Positive = std::numeric_limits<uint64_t>::max() };
    std::cout << "Underlying type of enum Enum { Negative = std::numeric_limits<uint64_t>::min(), Positive = std::numeric_limits<uint64_t>::max() }: ";
    using UnderlyingType = std::underlying_type_t<Enum>;
    std::cout << getPrettyTypeName<UnderlyingType>() << ".\n";
  }
  {
    enum Enum { Negative = -1, Positive = std::numeric_limits<uint64_t>::max() };
    std::cout << "Underlying type of enum Enum { Negative = -1, Positive = std::numeric_limits<uint64_t>::max() }: ";
    using UnderlyingType = std::underlying_type_t<Enum>;
    std::cout << getPrettyTypeName<UnderlyingType>() << ".\n";
  }
}

void testScopedEnumsUnderlyingTypes() {
  std::cout << "Testing the underlying types of scoped enums.\n";
  {
    enum class Enum { Value };
    std::cout << "Underlying type of enum class Enum { Value }: ";
    using UnderlyingType = std::underlying_type_t<Enum>;
    std::cout << getPrettyTypeName<UnderlyingType>() << ".\n";
  }
  {
    enum class Enum { Negative = -1, Positive = 1 };
    std::cout << "Underlying type of enum class Enum { Negative = -1, Positive = 1 }: ";
    using UnderlyingType = std::underlying_type_t<Enum>;
    std::cout << getPrettyTypeName<UnderlyingType>() << ".\n";
  }
}

void testUnderlyingEnumTypes() {
  testUnscopedEnumsUnderlyingTypes();
  testScopedEnumsUnderlyingTypes();
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

[[nodiscard]] int main() {
  testVectorAssignment();
  testVectorAllocationsAndFreesWithBlocks();
  testInsertWithConflictingKeyInUnorderedMap();
  testUnorderedSetGrowth();
  testUnderlyingEnumTypes();
  testSmallStringOptimizationSize();
  testPushBackAndEmplaceBackAllocations();
  testSharedPointerMemoryAllocations();
  return EXIT_SUCCESS;
}
} // namespace Experiments

int main() { return Experiments::main(); }
