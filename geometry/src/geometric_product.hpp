#pragma once

#include "geometry/expression_template.hpp"
#include "geometry/src/get.hpp"
#include "geometry/type_metaprogramming.hpp"

#include <type_traits>

namespace geometry {

inline constexpr class
{
  template <class A>
  struct null_generator
  {
    using algebra_type = A;

    template <class T>
    struct expr : std::false_type
    {};

    template <template <class...> class op, class... Args>
    struct expr<op<expression_template::multiplies, Args...>>
        : std::bool_constant<
              tmp::convert_to_sequence_t<
                  tmp::flatten_t<tmp::list<tmp::convert_to_list_t<
                      std::decay_t<typename Args::eval_type>,
                      tmp::list>...>>,
                  algebra_type::template reified_blade_coefficient>::value == 0>
    {};
  };

  template <template <class> class pred, class T>
  static constexpr auto prune_if(const T& arg)
  {
    if constexpr (pred<T>::value) {
      return std::tuple{};
    } else {
      return std::tuple{arg};
    }
  }

  template <template <class> class pred, std::size_t... Is, class... Ts>
  static constexpr auto
  prune_args(std::index_sequence<Is...>, const std::tuple<Ts...>& args)
  {
    return std::tuple_cat(prune_if<pred>(std::get<Is>(args))...);
  }

  template <
      template <class>
      class pred,
      template <class...>
      class op,
      class... Args,
      class T>
  static constexpr auto
  prune(op<expression_template::plus, Args...> expr, T empty_value)
  {
    auto args = prune_args<pred>(std::index_sequence_for<Args...>{}, expr.args);

    if constexpr (std::tuple_size_v<decltype(args)> == 0) {
      return empty_value;
    } else if constexpr (std::tuple_size_v<decltype(args)> == 1) {
      return std::get<0>(args);
    } else {
      using pruned_expr_type = tmp::rebind_into_t<
          tmp::flatten_t<
              tmp::list<tmp::list<expression_template::plus>, decltype(args)>>,
          op>;

      return pruned_expr_type{args};
    }
  }

  template <template <class> class pred, class Expr, class T>
  static constexpr auto prune(Expr expr, T empty_value)
  {
    if constexpr (pred<Expr>::value) {
      return empty_value;
    } else {
      return expr;
    }
  }

  template <
      class... Ts,
      template <class...>
      class multivector,
      class... B1s,
      class... B2s,
      class T>
  static constexpr auto
  impl(tmp::list<Ts...>,
       const multivector<B1s...>& x,
       const multivector<B2s...>& y,
       const T& zero)
  {
    using expression_template::leaf;
    using algebra_type = typename multivector<B1s...>::algebra_type;

    auto z =
        ((leaf(get<typename Ts::first_type>(x)) *
          leaf(get<typename Ts::second_type>(y))) +
         ...);

    return eval(
        prune<null_generator<algebra_type>::template expr>(z, leaf(zero)));
  }

public:
  template <
      template <class...>
      class multivector,
      class... B1s,
      class... B2s,
      class T = typename multivector<B1s...>::algebra_type::template blade<>>
  constexpr auto operator()(
      const multivector<B1s...>& x,
      const multivector<B2s...>& y,
      const T& zero = {}) const
  {
    return impl(
        tmp::cartesian_product_t<tmp::list<B1s...>, tmp::list<B2s...>>{},
        x,
        y,
        zero);
  }
} geometric_product{};

}  // namespace geometry
