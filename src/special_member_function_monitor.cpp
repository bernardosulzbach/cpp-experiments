#include "special_member_function_monitor.hpp"

namespace Experiments {
U64 SpecialMemberFunctionMonitor::constructor = 0;
U64 SpecialMemberFunctionMonitor::copyConstructor = 0;
U64 SpecialMemberFunctionMonitor::moveConstructor = 0;
U64 SpecialMemberFunctionMonitor::copyAssignment = 0;
U64 SpecialMemberFunctionMonitor::moveAssignment = 0;
U64 SpecialMemberFunctionMonitor::destructor = 0;

SpecialMemberFunctionMonitor::SpecialMemberFunctionMonitor() { constructor++; }

SpecialMemberFunctionMonitor::SpecialMemberFunctionMonitor(const SpecialMemberFunctionMonitor &rhs) noexcept { copyConstructor++; }

SpecialMemberFunctionMonitor::SpecialMemberFunctionMonitor(SpecialMemberFunctionMonitor &&rhs) noexcept { moveConstructor++; }

SpecialMemberFunctionMonitor &SpecialMemberFunctionMonitor::operator=(const SpecialMemberFunctionMonitor &rhs) noexcept {
  copyAssignment++;
  return *this;
}

SpecialMemberFunctionMonitor &SpecialMemberFunctionMonitor::operator=(SpecialMemberFunctionMonitor &&rhs) noexcept {
  moveAssignment++;
  return *this;
}

SpecialMemberFunctionMonitor::~SpecialMemberFunctionMonitor() { destructor++; }

static void addCallCountString(std::vector<std::string> &callCountStrings, const U64 callCount, const std::string &what) {
  if (callCount == 0) {
    return;
  }
  std::string string = std::to_string(callCount);
  string += " call";
  if (callCount > 1) {
    string += "s";
  }
  string += " to the ";
  string += what;
  callCountStrings.push_back(std::move(string));
}

void SpecialMemberFunctionMonitor::print() {
  std::vector<std::string> callCountStrings;
  addCallCountString(callCountStrings, constructor, "constructor");
  addCallCountString(callCountStrings, copyConstructor, "copy constructor");
  addCallCountString(callCountStrings, moveConstructor, "move constructor");
  addCallCountString(callCountStrings, copyAssignment, "copy assignment operator");
  addCallCountString(callCountStrings, moveAssignment, "move assignment operator");
  addCallCountString(callCountStrings, destructor, "destructor");
  std::cout << "made ";
  if (callCountStrings.empty()) {
    std::cout << "no calls";
  } else {
    std::cout << enumerate(callCountStrings);
  }
  std::cout << ".";
}

void SpecialMemberFunctionMonitor::reset() {
  constructor = 0;
  copyConstructor = 0;
  moveConstructor = 0;
  copyAssignment = 0;
  moveAssignment = 0;
  destructor = 0;
}
} // namespace Experiments
