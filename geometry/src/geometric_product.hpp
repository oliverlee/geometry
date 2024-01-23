#pragma once

#include "geometry/expression_template.hpp"
#include "geometry/src/common_algebra_type.hpp"
#include "geometry/src/detail/multivector_product.hpp"
#include "geometry/src/detail/pending_dimensions_list.hpp"
#include "geometry/type_metaprogramming.hpp"

#include <type_traits>

namespace geometry {
namespace detail {

inline constexpr class
{
public:
  template <
      class T1,
      class T2,
      class D1 = std::decay_t<typename T1::eval_type>,
      class D2 = std::decay_t<typename T2::eval_type>,
      class Algebra = common_algebra_type_t<D1, D2>,
      class L = detail::pending_dimensions_list_t<D1, D2>>
  constexpr auto
  operator()(expression_template::op<expression_template::multiplies, T1, T2>)
      const -> std::bool_constant<
          tmp::convert_to_sequence_t<
              L,
              Algebra::template reified_blade_coefficient>::value == 0>
  {
    return {};
  }
} null_generator{};

}  // namespace detail

/// geometric product
///
/// @{

inline constexpr auto geometric_product =  //
    detail::multivector_product_with(      //
        detail::null_generator,
        [](auto algebra) {
          return typename decltype(algebra)::template blade<>{};
        });

template <class T1, class T2, class A = common_algebra_type_t<T1, T2>>
[[nodiscard]]
constexpr auto
operator*(const T1& x, const T2& y)
{
  return geometric_product(x, y);
}

/// @}

}  // namespace geometry
