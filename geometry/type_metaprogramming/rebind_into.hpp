#pragma once

namespace geometry::type_metaprogramming {

template <class L, template <class...> class list2>
struct rebind_into
{};
template <
    class... Ts,
    template <class...>
    class list1,
    template <class...>
    class list2>
struct rebind_into<list1<Ts...>, list2>
{
  using type = list2<Ts...>;
};

template <class L, template <class...> class list2>
using rebind_into_t = typename rebind_into<L, list2>::type;

}  // namespace geometry::type_metaprogramming
