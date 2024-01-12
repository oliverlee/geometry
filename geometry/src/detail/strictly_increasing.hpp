#pragma once

#include <array>
#include <cstddef>
#include <utility>

namespace geometry::detail {

inline constexpr class
{
  template <class T>
  static constexpr auto impl(std::index_sequence<>, const std::array<T, 1>&)
  {
    return true;
  }
  template <
      std::size_t... Is,
      class T,
      class = std::enable_if_t<(sizeof...(Is) != 0)>>
  static constexpr auto
  impl(std::index_sequence<Is...>,
       const std::array<T, sizeof...(Is) + 1>& values)
  {
    return ((std::get<Is>(values) < std::get<Is + 1>(values)) and ...);
  }

public:
  // 0-args
  constexpr auto operator()() const { return true; }
  // 1+-args
  template <class... Ts>
  constexpr auto operator()(std::size_t t0, Ts... ts) const
  {
    static_assert((std::is_same_v<std::size_t, Ts> and ...));
    return impl(std::index_sequence_for<Ts...>{}, std::array{t0, ts...});
  }
} strictly_increasing{};

}  // namespace geometry::detail
