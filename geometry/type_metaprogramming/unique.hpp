#pragma once

#include "geometry/type_metaprogramming/empty.hpp"

namespace geometry::type_metaprogramming {

template <class In, class Out = empty_t<In>>
struct unique
{
  using type = Out;
};

template <template <class...> class list, class T, class... Rs>
struct unique<list<T>, list<Rs...>> : unique<list<>, list<Rs..., T>>
{};

template <template <class...> class list, class T, class... Ts, class... Rs>
struct unique<list<T, T, Ts...>, list<Rs...>>
    : unique<list<T, Ts...>, list<Rs...>>
{};

template <
    template <class...>
    class list,
    class T0,
    class T1,
    class... Ts,
    class... Rs>
struct unique<list<T0, T1, Ts...>, list<Rs...>>
    : unique<list<T1, Ts...>, list<Rs..., T0>>
{};

template <class L>
using unique_t = typename unique<L>::type;

}  // namespace geometry::type_metaprogramming
