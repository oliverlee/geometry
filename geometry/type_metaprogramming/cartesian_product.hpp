#pragma once

#include "geometry/type_metaprogramming/list.hpp"

#include <cstddef>
#include <tuple>
#include <utility>

namespace geometry::type_metaprogramming {

template <class L1, class L2>
struct cartesian_product;

template <
    template <class...>
    class list1,
    class... Ts,
    template <class...>
    class list2,
    class... Us>
struct cartesian_product<list1<Ts...>, list2<Us...>>
{
  template <std::size_t... Is>
  static auto impl(std::index_sequence<Is...>) -> list<std::pair<
      std::tuple_element_t<(Is / sizeof...(Us)), std::tuple<Ts...>>,
      std::tuple_element_t<(Is % sizeof...(Us)), std::tuple<Us...>>>...>;

  using type =
      decltype(impl(std::make_index_sequence<sizeof...(Ts) * sizeof...(Us)>{}));
};

template <class L1, class L2>
using cartesian_product_t = typename cartesian_product<L1, L2>::type;

}  // namespace geometry::type_metaprogramming
