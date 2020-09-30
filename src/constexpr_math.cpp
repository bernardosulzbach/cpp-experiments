#include "constexpr_math.hpp"

#include <cmath>
#include <iostream>
#include <type_traits>

#include "formatting.hpp"

namespace Experiments {
// This constexpr check was taken from https://stackoverflow.com/a/49943482/3271844.

// The following turns anything (in our case a value of A) into an int.
// This is necessary because non-type template arguments must be integral (likely to change with C++20).
template <class T> constexpr int make_int(T &&) { return 0; }

// Helper to turn some function type (e.g. int(float)) into a function pointer type (e.g. int (*)(float)).
template <class T> struct signature_from;
template <class R, class... Args> struct signature_from<R(Args...)> { using type = R (*)(Args...); };

// See std::void_t for the idea. This does it for ints instead of types.
template <int...> using void_from_int = void;

// The fallback case: F is not a function pointer to a constexpr function.
template <class T, typename signature_from<T>::type F, class = void_from_int<>> struct is_constexpr_base { static constexpr bool value = false; };

// If void_from_int<make_int(F(Args()...))> doesn't lead to a substitution failure, then this is the preferred specialization.
// In that case F must be a function pointer to a constexpr function.
// If it is not, it could not be used in a template argument.
template <class R, class... Args, typename signature_from<R(Args...)>::type F> struct is_constexpr_base<R(Args...), F, void_from_int<make_int(F(Args()...))>> {
  static constexpr bool value = true;
};

template <auto F> struct isConstexpr : is_constexpr_base<std::remove_pointer_t<decltype(F)>, F> {};

void testConstexprMath() {
  std::cout << "Testing <cmath> for constexpr-ness:\n";
  std::cout << Warning << "This is a work-in-progress and the results don't seem to be totally accurate.\n";

  std::cout << Indentation << "std::sqrt(float): " << isConstexpr<static_cast<float (*)(float)>(std::sqrt)>::value << "\n";
  std::cout << Indentation << "std::sqrt(double): " << isConstexpr<static_cast<double (*)(double)>(std::sqrt)>::value << "\n";
  std::cout << Indentation << "std::sqrt(long double): " << isConstexpr<static_cast<long double (*)(long double)>(std::sqrt)>::value << "\n";
  std::cout << Indentation << "std::sqrt(int): " << isConstexpr<static_cast<double (*)(int)>(std::sqrt)>::value << "\n";

  std::cout << Indentation << "std::log(float): " << isConstexpr<static_cast<float (*)(float)>(std::log)>::value << "\n";
  std::cout << Indentation << "std::log(double): " << isConstexpr<static_cast<double (*)(double)>(std::log)>::value << "\n";
  std::cout << Indentation << "std::log(long double): " << isConstexpr<static_cast<long double (*)(long double)>(std::log)>::value << "\n";
  std::cout << Indentation << "std::log(int): " << isConstexpr<static_cast<double (*)(int)>(std::log)>::value << "\n";

  std::cout << Indentation << "std::sin(float): " << isConstexpr<static_cast<float (*)(float)>(std::sin)>::value << "\n";
  std::cout << Indentation << "std::sin(double): " << isConstexpr<static_cast<double (*)(double)>(std::sin)>::value << "\n";
  std::cout << Indentation << "std::sin(long double): " << isConstexpr<static_cast<long double (*)(long double)>(std::sin)>::value << "\n";
  std::cout << Indentation << "std::sin(int): " << isConstexpr<static_cast<double (*)(int)>(std::sin)>::value << "\n";
}
} // namespace Experiments