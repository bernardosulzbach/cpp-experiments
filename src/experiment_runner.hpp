#pragma once

#include <functional>
#include <utility>

namespace Experiments {
class ExperimentRunner {
  std::function<void()> experimentFunction;

public:
  explicit ExperimentRunner(std::function<void()> function) : experimentFunction(std::move(function)) {}

  void run() noexcept;
};
} // namespace Experiments
