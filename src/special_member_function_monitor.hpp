#pragma once

#include <iostream>

#include "enumerate.hpp"
#include "types.hpp"

namespace Experiments {

class SpecialMemberFunctionMonitor {
  static U64 constructor;
  static U64 copyConstructor;
  static U64 moveConstructor;
  static U64 copyAssignment;
  static U64 moveAssignment;
  static U64 destructor;

public:
  SpecialMemberFunctionMonitor();
  SpecialMemberFunctionMonitor(const SpecialMemberFunctionMonitor &rhs) noexcept;
  SpecialMemberFunctionMonitor(SpecialMemberFunctionMonitor &&rhs) noexcept;
  SpecialMemberFunctionMonitor &operator=(const SpecialMemberFunctionMonitor &rhs) noexcept;
  SpecialMemberFunctionMonitor &operator=(SpecialMemberFunctionMonitor &&rhs) noexcept;
  virtual ~SpecialMemberFunctionMonitor();

  static void print();

  static void reset();
};
} // namespace Experiments
