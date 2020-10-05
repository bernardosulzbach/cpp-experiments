#pragma once

namespace Experiments {
/**
 * According to the C++ standard,
 *
 * "Nonstatic data members of a (non-union) class declared without an intervening access-specifier are allocated so that later members have higher addresses
 * within a class object. The order of allocation of nonstatic data members separated by an access-specifier is unspecified."
 *
 * This test aims to verify whether or not the compiler is performing struct reordering.
 * */
void testStructReordering() noexcept;
} // namespace Experiments
