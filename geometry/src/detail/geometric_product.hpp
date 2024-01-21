#pragma once

#include "geometry/expression_template.hpp"
#include "geometry/type_metaprogramming.hpp"

namespace geometry::detail {

template <class T>
static auto get() -> void;

inline constexpr class
{
  template <
      class... Ts,
      template <class...>
      class multivector,
      class... B1s,
      class... B2s>
  static constexpr auto
  impl(tmp::list<Ts...>,
       const multivector<B1s...>& x,
       const multivector<B2s...>& y)
  {
    using expression_template::leaf;
    return eval((
        (leaf(get<typename Ts::first_type>(x)) *
         leaf(get<typename Ts::second_type>(y))) +
        ...));
  }

public:
  template <template <class...> class multivector, class... B1s, class... B2s>
  constexpr auto
  operator()(const multivector<B1s...>& x, const multivector<B2s...>& y) const
  {
    return impl(
        tmp::cartesian_product_t<tmp::list<B1s...>, tmp::list<B2s...>>{}, x, y);
  }
} geometric_product{};

}  // namespace geometry::detail
