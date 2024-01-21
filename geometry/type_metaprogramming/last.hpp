#pragma once

#include <tuple>

namespace geometry::type_metaprogramming {

template <class L>
struct last;

template <class... Ts, template <class...> class list>
struct last<list<Ts...>>
{
  using type = std::tuple_element_t<sizeof...(Ts) - 1, std::tuple<Ts...>>;
};

template <class L>
using last_t = typename last<L>::type;

}  // namespace geometry::type_metaprogramming
