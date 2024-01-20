#pragma once

#include "geometry/expression_template/enable_ring_operations.hpp"

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

namespace geometry::expression_template {

namespace detail {

template <std::size_t... Is, class F, class Tuple>
constexpr decltype(auto)
eval(std::index_sequence<Is...>, const F& func, const Tuple& args)
{
  return func(eval(std::get<Is>(args))...);
}

}  // namespace detail

template <class F, class... Args>
struct op : enable_ring_operations<op<F, Args...>>
{
  using func_type = F;
  using eval_type = std::invoke_result_t<const F&, typename Args::eval_type...>;

  template <class... Ts>
  using expression = op<Ts...>;

  template <class... Ts>
  using rebind_args = op<F, Ts...>;

  std::tuple<Args...> args;

  constexpr explicit op(Args... args) : args{args...} {}

  friend constexpr auto eval(op x) -> eval_type
  {
    return detail::eval(
        std::make_index_sequence<sizeof...(Args)>{}, F{}, x.args);
  }
};

}  // namespace geometry::expression_template
