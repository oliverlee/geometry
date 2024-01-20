#pragma once

#include "geometry/expression_template/enable_ring_operations.hpp"
#include "geometry/expression_template/op.hpp"

namespace geometry::expression_template {

template <class T>
struct leaf : enable_ring_operations<leaf<T>>
{
  using func_type = void;
  using eval_type = const T&;

  template <class... Ts>
  using expression = op<Ts...>;

  const T& value;

  constexpr leaf(const T& value) : value{value} {}

  friend constexpr auto eval(leaf x) -> eval_type { return x.value; }
};

}  // namespace geometry::expression_template
