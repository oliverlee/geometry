#pragma once

#include "geometry/type_metaprogramming.hpp"

namespace geometry {

template <class... Ts>
using type_list = tmp::list<Ts...>;

}  // namespace geometry
