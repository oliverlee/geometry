#pragma once

#include "geometry/expression_template.hpp"
#include "geometry/src/common_algebra_type.hpp"
#include "geometry/src/get.hpp"
#include "geometry/src/to_multivector.hpp"
#include "geometry/type_metaprogramming.hpp"

#include <cstddef>
#include <tuple>
#include <type_traits>

namespace geometry::detail {

inline constexpr class
{
  template <class ExprFilter, class T>
  static constexpr auto prune_if(ExprFilter pred, T arg)
  {
    if constexpr (decltype(pred(arg))::value) {
      return std::tuple{};
    } else {
      return std::tuple{arg};
    }
  }

  template <std::size_t... Is, class ExprFilter, class... Ts>
  static constexpr auto prune_args(
      std::index_sequence<Is...>,
      ExprFilter pred,
      const std::tuple<Ts...>& args)
  {
    return std::tuple_cat(prune_if(pred, std::get<Is>(args))...);
  }

  template <class ExprFilter, class... Ts, class U>
  static constexpr auto
  prune(ExprFilter pred,
        expression_template::op<expression_template::plus, Ts...> expr,
        U empty)
  {
    auto args = prune_args(std::index_sequence_for<Ts...>{}, pred, expr.args);

    constexpr auto arg_count = std::tuple_size_v<decltype(args)>;

    if constexpr (arg_count == 0) {
      return empty;
    } else if constexpr (arg_count == 1) {
      return std::get<0>(args);
    } else {
      using pruned_expr_type = tmp::rebind_into_t<
          tmp::flatten_t<
              tmp::list<tmp::list<expression_template::plus>, decltype(args)>>,
          expression_template::op>;

      return pruned_expr_type{args};
    }
  }

  template <class ExprFilter, class Expr, class U>
  static constexpr auto prune(ExprFilter pred, Expr expr, U empty)
  {
    if constexpr (decltype(pred(expr))::value) {
      return empty;
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
      class ExprFilter,
      class T>
  static constexpr auto
  impl(tmp::list<Ts...>,
       const multivector<B1s...>& x,
       const multivector<B2s...>& y,
       ExprFilter pred,
       const T& zero)
  {
    using expression_template::leaf;

    auto z =
        ((leaf(get<typename Ts::first_type>(x)) *
          leaf(get<typename Ts::second_type>(y))) +
         ...);

    return eval(prune(pred, z, leaf(zero)));
  }

public:
  template <
      template <class...>
      class multivector,
      class... B1s,
      class... B2s,
      class ExprFilter,
      class T>
  [[nodiscard]]
  constexpr auto
  operator()(
      const multivector<B1s...>& x,
      const multivector<B2s...>& y,
      ExprFilter pred,
      const T& zero) const
  {
    return impl(
        tmp::cartesian_product_t<tmp::list<B1s...>, tmp::list<B2s...>>{},
        x,
        y,
        pred,
        zero);
  }
} multivector_product{};

template <class Prune, class Zero>
class product_fn : Zero
{
public:
  constexpr product_fn(Prune, Zero zero_fn) : Zero{std::move(zero_fn)} {}

  template <class T1, class T2, class Algebra = common_algebra_type_t<T1, T2>>
  constexpr auto operator()(const T1& x, const T2& y) const
  {
    const auto& zero_for = static_cast<const Zero&>(*this);
    return multivector_product(
        to_multivector<Algebra>(x),
        to_multivector<Algebra>(y),
        Prune{},
        zero_for(Algebra{}));
  }
};

inline constexpr auto multivector_product_with = [](auto prune, auto zero_for) {
  return product_fn{prune, std::move(zero_for)};
};

}  // namespace geometry::detail
