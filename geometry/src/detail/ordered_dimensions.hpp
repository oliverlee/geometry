#pragma once

#include "geometry/src/detail/factorial.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <iterator>

namespace geometry::detail {

// For N = 3, K = 2
// {
//   {0, 1},
//   {0, 2},
//   {1, 2},
// }
template <std::size_t N, std::size_t K>
inline constexpr auto ordered_dimensions = [] {
  static_assert(N >= K);

  using dimensions_type = std::array<std::size_t, K>;

  constexpr auto iota = [](auto first, auto last, auto start) {
    while (first != last) {
      *first++ = start++;
    }
  };

  constexpr auto advance = [=](dimensions_type& x) {
    assert(not x.empty());

    auto first = std::prev(x.end());
    while (
        *first + static_cast<std::size_t>(std::distance(first, x.end())) == N) {
      if (first == x.cbegin()) {
        return;
      }
      --first;
    }

    iota(first, x.end(), ++*first);
  };

  constexpr auto n_choose_k = factorial(N) / factorial(K) / factorial(N - K);
  auto dimensions = std::array<dimensions_type, n_choose_k>{};

  if (K == 0) {
    return dimensions;
  }

  auto x = dimensions_type{};
  iota(x.begin(), x.end(), std::size_t{});

  for (auto& d : dimensions) {
    d = x;
    advance(x);
  }

  return dimensions;
}();

}  // namespace geometry::detail
