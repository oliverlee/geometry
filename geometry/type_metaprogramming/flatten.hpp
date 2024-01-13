#pragma once

namespace geometry::type_metaprogramming {

template <class R, class... Ts>
struct flatten_;

template <class R>
struct flatten_<R>
{
  using type = R;
};

template <
    template <class...>
    class list,
    class... Rs,
    template <class...>
    class list2,
    class... Ts,
    class... Ls>
struct flatten_<list<Rs...>, list2<Ts...>, Ls...>
    : flatten_<list<Rs..., Ts...>, Ls...>
{};

template <class L>
struct flatten;

template <template <class...> class list, class... Ls>
struct flatten<list<Ls...>> : flatten_<list<>, Ls...>
{};

template <class L>
using flatten_t = typename flatten<L>::type;

}  // namespace geometry::type_metaprogramming
