#pragma once

namespace geometry::type_metaprogramming {

template <class T>
struct get_type
{
  using type = typename T::type;
};

template <class T>
using get_type_t = typename get_type<T>::type;

}  // namespace geometry::type_metaprogramming
