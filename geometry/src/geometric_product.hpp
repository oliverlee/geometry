#pragma once

#include "geometry/expression_template.hpp"
#include "geometry/src/common_algebra_type.hpp"
#include "geometry/src/detail/multivector_product.hpp"
#include "geometry/src/to_multivector.hpp"
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
      class Algebra = common_algebra_type_t<D1, D2>>
  constexpr auto
  operator()(expression_template::op<expression_template::multiplies, T1, T2>)
      const -> std::bool_constant<
          tmp::convert_to_sequence_t<
              tmp::flatten_t<tmp::list<tmp::convert_to_list_t<D1, tmp::list>,
                                       tmp::convert_to_list_t<D2, tmp::list>>>,
              Algebra::template reified_blade_coefficient>::value == 0>
  {
    return {};
  }
} null_generator{};

}  // namespace detail

inline constexpr auto geometric_product = [](const auto& x, const auto& y) {
  using algebra_type = common_algebra_type_t<
      std::decay_t<decltype(x)>,
      std::decay_t<decltype(y)>>;
  constexpr auto zero = typename algebra_type::template blade<>{};

  return detail::multivector_product(
      to_multivector<algebra_type>(x),
      to_multivector<algebra_type>(y),
      zero,
      detail::null_generator);
};

}  // namespace geometry
