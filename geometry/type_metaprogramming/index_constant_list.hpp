#pragma once

#include "geometry/type_metaprogramming/list.hpp"

#include <cstddef>
#include <utility>

namespace geometry::type_metaprogramming {

template <std::size_t... Is>
using index_constant_list = list<std::integral_constant<std::size_t, Is>...>;

}  // namespace geometry::type_metaprogramming
