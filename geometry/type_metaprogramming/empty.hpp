#pragma once

namespace geometry::type_metaprogramming {

template <class L>
struct empty;

template <template <class...> class list, class... Ts>
struct empty<list<Ts...>>
{
  using type = list<>;
};

template <class L>
using empty_t = typename empty<L>::type;

}  // namespace geometry::type_metaprogramming
