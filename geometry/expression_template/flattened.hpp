#pragma once

#include "geometry/expression_template/is_associative.hpp"

#include <tuple>
#include <type_traits>
#include <utility>

namespace geometry::expression_template {

inline constexpr class
{
  template <class F, template <class...> class op, class... Args>
  static constexpr auto extract_args_from(const op<F, Args...>& expr)
  {
    return expr.args;
  }
  template <class F, class T>
  static constexpr auto extract_args_from(const T& expr)
  {
    return std::tuple{expr};
  }

  template <class Op, std::size_t... Is, class Args>
  static constexpr auto join(std::index_sequence<Is...>, const Args& args)
  {
    return typename Op::template rebind_args<std::tuple_element_t<Is, Args>...>{
        std::get<Is>(args)...};
  }

  template <class Op, std::size_t... Is, class Args>
  static constexpr auto split(std::index_sequence<Is...>, const Args& args)
  {
    using F = typename Op::func_type;
    auto all_args = std::tuple_cat(extract_args_from<F>(std::get<Is>(args))...);

    return join<Op>(
        std::make_index_sequence<std::tuple_size_v<decltype(all_args)>>{},
        all_args);
  }

public:
  template <template <class...> class op, class F, class... Args>
  constexpr auto operator()(const op<F, Args...>& expr) const
  {
    if constexpr (
        is_associative_v<F> and
        (std::is_same_v<F, typename Args::func_type> or ...)) {
      return split<op<F, Args...>>(
          std::index_sequence_for<Args...>{}, expr.args);
    } else {
      return expr;
    }
  }

} flattened{};

}  // namespace geometry::expression_template
