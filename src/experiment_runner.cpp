#include "experiment_runner.hpp"

#include <iostream>
#include <stdexcept>

#include "formatting.hpp"

namespace Experiments {
void ExperimentRunner::run() noexcept {
  try {
    experimentFunction();
  } catch (const std::exception &any) {
    std::cerr << "An experiment threw:\n";
    std::cerr << Indentation << getPrettyTypeName<decltype(any)>() << ": " << any.what() << "\n";
  }
}
} // namespace Experiments