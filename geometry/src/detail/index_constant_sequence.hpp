#pragma once

#include "geometry/src/type_list.hpp"

#include <cstddef>
#include <utility>

namespace geometry::detail {

template <std::size_t N>
struct index_constant_sequence
{
  template <std::size_t... Is>
  static auto to_type_list(std::index_sequence<Is...>)
      -> type_list<std::integral_constant<std::size_t, Is>...>;

  using type = decltype(to_type_list(std::make_index_sequence<N>{}));
};

template <std::size_t N>
using index_constant_sequence_t = typename index_constant_sequence<N>::type;

}  // namespace geometry::detail
