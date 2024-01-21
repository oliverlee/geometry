#pragma once

#include "geometry/type_metaprogramming.hpp"

namespace geometry::detail {

template <class T1, class T2>
struct pending_dimensions_list
{
  using type = tmp::flatten_t<tmp::list<
      tmp::convert_to_list_t<T1, tmp::list>,
      tmp::convert_to_list_t<T2, tmp::list>>>;
};

template <class T1, class T2>
using pending_dimensions_list_t =
    typename pending_dimensions_list<T1, T2>::type;

}  // namespace geometry::detail
