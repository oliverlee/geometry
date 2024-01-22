#pragma once

#include "geometry/src/algebra_type.hpp"
#include "geometry/src/get.hpp"
#include "geometry/src/to_multivector.hpp"

namespace geometry::detail {

template <class D>
class bladewise_transform : D
{
  template <template <class...> class multivector, class... Bs>
  constexpr auto impl(const multivector<Bs...>& v) const
  {
    return (static_cast<const D&>(*this)(get<Bs>(v)) + ...);
  }

public:
  template <class V, class A = algebra_type_t<V>>
  constexpr auto operator()(const V& v) const
  {
    return impl(to_multivector<A>(v));
  }
};

}  // namespace geometry::detail
