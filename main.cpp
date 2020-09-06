#include <atomic>
#include <bitset>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using U8 = uint8_t;

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

int main() {
  testVectorAssignment();
  testVectorAllocationsAndFreesWithBlocks();
  testInsertWithConflictingKeyInUnorderedMap();
  testUnorderedSetGrowth();
  return 0;
}
