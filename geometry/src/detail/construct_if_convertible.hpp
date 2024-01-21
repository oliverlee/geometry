#pragma once

#include <type_traits>

namespace geometry::detail {

template <class S>
struct construct_if_convertible_fn
{
  template <
      class T,
      std::enable_if_t<std::is_constructible_v<S, const T&>, bool> = true>
  constexpr auto operator()(const T& t) const
  {
    return static_cast<S>(t);
  }
  template <
      class T,
      std::enable_if_t<not std::is_constructible_v<S, const T&>, bool> = true>
  constexpr auto& operator()(const T& t) const
  {
    return t;
  }
};

template <class S>
inline constexpr auto construct_if_convertible =
    construct_if_convertible_fn<S>{};

}  // namespace geometry::detail
