#pragma once

#include "geometry/src/algebra.hpp"
#include "geometry/src/detail/bladewise_transform.hpp"
#include "geometry/src/detail/pending_dimensions_list.hpp"
#include "geometry/type_metaprogramming.hpp"

#include <type_traits>

namespace geometry {
namespace detail {

class dual_fn
{
protected:
  template <bool b, class T>
  static constexpr auto negate_if(T value)
  {
    if constexpr (b) {
      return -value;
    } else {
      return value;
    }
  }

public:
  template <
      class T,
      class Algebra = algebra_type_t<T>,
      class = std::enable_if_t<Algebra::template is_blade_v<T>>>
  constexpr auto operator()(const T& x) const
  {
    using D = decltype(Algebra::i * x);

    // https://discourse.bivector.net/t/how-to-actually-compute-the-dual-and-confusions-about-wedge-product-in-pga/249/19
    //
    // A Guided Tour to the Plane-Based Geometric Algebra PGA
    // https://bivector.net/PGA4CS.pdf, eq 125
    // applies the dual on the right

    constexpr auto sign = tmp::convert_to_sequence_t<
        detail::pending_dimensions_list_t<T, D>,
        Algebra::template reified_blade_coefficient>::value;

    using dual_blade_type = tmp::convert_to_sequence_t<
        tmp::convert_to_list_t<D, tmp::list>,
        Algebra::dual_algebra_type::template blade>;

    return negate_if<sign == -1>(dual_blade_type{x.coefficient});
  }
};

class undual_fn : dual_fn
{
  template <class I>
  static constexpr auto is_odd(I value)
  {
    return value % 2 != 0;
  }

public:
  template <
      class T,
      class Algebra = algebra_type_t<T>,
      class = std::enable_if_t<Algebra::template is_blade_v<T>>>
  constexpr auto operator()(const T& x) const
  {
    // A Guided Tour to the Plane-Based Geometric Algebra PGA
    // https://bivector.net/PGA4CS.pdf, eq 132

    return negate_if<is_odd(T::grade * (Algebra::dimension - 1))>(
        dual_fn::operator()(x));
  }
};

}  // namespace detail

/// dual
///
inline constexpr auto dual = detail::bladewise_transform<detail::dual_fn>{};

/// undual
///
inline constexpr auto undual = detail::bladewise_transform<detail::undual_fn>{};

}  // namespace geometry
