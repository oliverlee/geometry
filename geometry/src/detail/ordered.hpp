#pragma once

#include <cstddef>
#include <tuple>

namespace geometry::detail {

template <class B>
class ordered
{
  template <
      template <std::size_t...>
      class blade,
      std::size_t... Is,
      std::size_t... Js>
  static constexpr auto
  compare_dimensions(ordered<blade<Is...>>, ordered<blade<Js...>>)
  {
    return std::tuple{Is...} < std::tuple{Js...};
  }

  template <class B2>
  friend constexpr auto operator<(ordered x, ordered<B2> y) -> bool
  {
    if constexpr (B::grade == B2::grade) {
      return compare_dimensions(x, y);
    } else {
      return B::grade < B2::grade;
    }
  }
};

}  // namespace geometry::detail
