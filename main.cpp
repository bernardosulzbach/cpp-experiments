#include <atomic>
#include <bitset>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <boost/type_index.hpp>

using U8 = std::uint8_t;

template <typename T> std::string getPrettyTypeName() {
  std::string name = boost::typeindex::type_id<T>().pretty_name();
  name += " (";
  name += std::to_string(8U * sizeof(T));
  name += " bits)";
  return name;
}

static std::atomic<bool> writingAllocationMessages = false;
static std::atomic<bool> writingDeallocationMessages = false;

static std::atomic<std::size_t> allocationCount = 0;

template <typename T> bool hasChanged(T &lastT, const T newT) {
  if (lastT == newT) {
    return false;
  }
  lastT = newT;
  return true;
}

void *operator new(const std::size_t size) {
  if (allocationCount == std::numeric_limits<decltype(allocationCount)::value_type>::max()) {
    throw std::bad_alloc();
  }
  if (writingAllocationMessages) {
    std::cout << "Made an allocation of size " << size << ".\n";
  }
  allocationCount++;
  return std::malloc(size);
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

void disableAllocationMessages() { writingAllocationMessages = false; }

void enableAllocationMessages() { writingAllocationMessages = true; }

void disableDeallocationMessages() { writingDeallocationMessages = false; }

void enableDeallocationMessages() { writingDeallocationMessages = true; }

class AllocationTrackerGuard {
  std::size_t allocationCountAtStart;

public:
  AllocationTrackerGuard(bool trackDeallocations) : allocationCountAtStart(allocationCount) {
    enableAllocationMessages();
    if (trackDeallocations) {
      enableDeallocationMessages();
    }
  }

  virtual ~AllocationTrackerGuard() {
    if (allocationCount == allocationCountAtStart) {
      std::cout << "Made no allocations.\n";
    }
    disableAllocationMessages();
    disableDeallocationMessages();
  }
};

void testVectorAssignment() {
  std::array<std::size_t, 3> sizes = {10, 100, 1000};
  for (const auto lhsSize : sizes) {
    for (const auto rhsSize : sizes) {
      std::vector<U8> lhs(lhsSize);
      std::vector<U8> rhs(rhsSize);
      std::cout << "Testing assigning vector of size " << rhsSize << " to a vector of size " << lhsSize << ". ";
      AllocationTrackerGuard allocationTrackerGuard(false);
      lhs = rhs;
    }
  }
}

void testVectorAllocationsAndFreesWithBlocks() {
  AllocationTrackerGuard allocationTrackerGuard(true);
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

int main() {
  testVectorAssignment();
  testVectorAllocationsAndFreesWithBlocks();
  testInsertWithConflictingKeyInUnorderedMap();
  testUnorderedSetGrowth();
  testUnderlyingEnumTypes();
  return 0;
}
