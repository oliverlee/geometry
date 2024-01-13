#pragma once

#include "geometry/type_metaprogramming/list.hpp"

#include <cstddef>
#include <utility>

namespace geometry::type_metaprogramming {

template <std::size_t... Is>
using index_constant_sequence =
    list<std::integral_constant<std::size_t, Is>...>;

template <class L>
struct into_index_constant_sequence;

template <std::size_t... Is>
struct into_index_constant_sequence<std::index_sequence<Is...>>
{
  using type = index_constant_sequence<Is...>;
};

template <std::size_t N>
using make_index_constant_sequence =
    typename into_index_constant_sequence<std::make_index_sequence<N>>::type;

}  // namespace geometry::type_metaprogramming
