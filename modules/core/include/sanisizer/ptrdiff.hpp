#ifndef SANISIZER_PTRDIFF_HPP
#define SANISIZER_PTRDIFF_HPP

#include <limits>
#include <utility>

#include "comparisons.hpp"
#include "utils.hpp"

/**
 * @file ptrdiff.hpp
 * @brief Safe pointer differences.
 */

namespace sanisizer {

/**
 * Check if differences between iterators can be safely represented in the iterator's difference type.
 * If `max_diff` is would overflow the difference type, an OverflowError is raised.
 *
 * @tparam Iterator_ Random access iterator that supports subtraction. 
 * @tparam MaxDiff_ Integer type for maximum difference between iterators.
 *
 * @param max_diff Maximum difference between iterators, typically derived from external knowledge (e.g., the array/container size).
 */
template<typename Iterator_, typename MaxDiff_>
void can_ptrdiff(MaxDiff_ max_diff) {
    typedef decltype(I(std::declval<Iterator_>() - std::declval<Iterator_>())) Diff;
    constexpr auto theoretical_max_diff = std::numeric_limits<Diff>::max();
    if constexpr(!is_greater_than_or_equal(theoretical_max_diff, std::numeric_limits<MaxDiff_>::max())) {
        if (!is_greater_than_or_equal(theoretical_max_diff, max_diff)) {
            throw OverflowError("potential overflow detected in sanisizer::can_ptrdiff");
        }
    }
}

// It is tempting to write a ptrdiff() function that checks each subtraction for overflow given 'start' and 'end' iterators.
// This could be implemented by checking if 'end >= start + theoretical_max_diff' when 'max_diff > theoretical_max_diff'.
// Unfortunately, this causes the most recent Clang to emit a warning when 'start' and 'end' are pointers to an array of objects of size > 2, 
// because it knows that 'theoretical_max_diff' cannot be allocated when ptrdiff_t is a signed version of size_t.
// It's not clear how we can avoid this warning as general iterators might have a smaller difference type that wouldn't hit such limitations.
//
// In addition, a safe ptrdiff() can't do much compile-time trickery to skip the run-time checks, at least not beyond what is already in can_ptrdiff().
// This is problematic for performance as our safe ptrdiff() may run in a tight loop.
// By comparison, can_ptrdiff() only needs to be run once to protect all subsequent differences.
// This comes at the theoretical cost of more false-positive errors if the differences could otherwise fit,
// but it's hard to imagine a case where we could guarantee that 'end - start' would never reach 'max_diff'... because then we could just set a lower 'max_diff'.

}

#endif
