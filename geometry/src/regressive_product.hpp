#pragma once

#include "geometry/src/common_algebra_type.hpp"
#include "geometry/src/dual.hpp"

namespace geometry {

/// regressive product (vee)
///
/// @{

inline constexpr struct
{
  template <class T1, class T2, class A = common_algebra_type_t<T1, T2>>
  [[nodiscard]]
  constexpr auto
  operator()(const T1& x, const T2& y) const
  {
    return undual(dual(x) ^ dual(y));
  }
} regressive_product{};

template <class T1, class T2, class A = common_algebra_type_t<T1, T2>>
[[nodiscard]]
constexpr auto
operator&(const T1& x, const T2& y)
{
  return regressive_product(x, y);
}

/// @}

}  // namespace geometry
