#pragma once

namespace geometry::detail {

template <class R, class... Ts>
struct type_flatten_
{};

template <class R>
struct type_flatten_<R>
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
struct type_flatten_<list<Rs...>, list2<Ts...>, Ls...>
    : type_flatten_<list<Rs..., Ts...>, Ls...>
{};

template <class L>
struct type_flatten
{};
template <template <class...> class list, class... Ls>
struct type_flatten<list<Ls...>> : type_flatten_<list<>, Ls...>
{};

template <class L>
using type_flatten_t = typename type_flatten<L>::type;

}  // namespace geometry::detail
