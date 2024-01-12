#pragma once

#include <cstddef>
#include <tuple>
#include <utility>

namespace geometry::detail {

inline constexpr class
{
  template <std::size_t... Is, class T>
  static constexpr auto impl(std::index_sequence<Is...>, const T& values)
  {
    if constexpr (sizeof...(Is) == 0) {
      return true;
    } else {
      return (((std::get<Is>(values)) < (std::get<Is + 1>(values))) and ...);
    }
  }

public:
  // 0-args
  constexpr auto operator()() const { return true; }
  // 1+-args
  template <class T0, class... Ts>
  constexpr auto operator()(T0 t0, Ts... ts) const
  {
    return impl(std::index_sequence_for<Ts...>{}, std::tuple{t0, ts...});
  }
} strictly_increasing{};

}  // namespace geometry::detail
