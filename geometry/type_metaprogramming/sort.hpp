#pragma once

#include "geometry/type_metaprogramming/empty.hpp"

#include <cstddef>
#include <type_traits>
#include <utility>

namespace geometry::type_metaprogramming {

template <class In, class Out = empty_t<In>>
struct sort_swap_first;

template <template <class...> class list, class... Js>
struct sort_swap_first<list<>, list<Js...>>
{
  using type = list<Js...>;
};

template <template <class...> class list, class I0, class... Js>
struct sort_swap_first<list<I0>, list<Js...>>
{
  using type = list<Js..., I0>;
};
template <
    template <class...>
    class list,
    class I0,
    class I1,
    class... Is,
    class... Js>
struct sort_swap_first<list<I0, I1, Is...>, list<Js...>>
    : std::conditional_t<
          (I1{} < I0{}),
          sort_swap_first<list<>, list<Js..., I1, I0, Is...>>,
          sort_swap_first<list<I1, Is...>, list<Js..., I0>>>
{};

template <class Seq>
using sort_swap_first_t = typename sort_swap_first<Seq>::type;

template <class Seq, std::size_t Swaps = 0, class = void>
struct sort_swap : sort_swap<sort_swap_first_t<Seq>, Swaps + 1>
{};

template <template <class...> class list, class... Is, std::size_t Swaps>
struct sort_swap<
    list<Is...>,
    Swaps,
    std::enable_if_t<
        std::is_same_v<list<Is...>, sort_swap_first_t<list<Is...>>>>>
{
  using type = list<Is...>;
  static constexpr auto swap_count = Swaps;
};

template <class L>
struct sort
{
  using type = typename sort_swap<L>::type;
};

template <class L>
using sort_t = typename sort<L>::type;

template <class L>
struct sort_swap_count
    : std::integral_constant<std::size_t, sort_swap<L>::swap_count>
{};

template <class L>
inline constexpr auto sort_swap_count_v = sort_swap_count<L>::value;

}  // namespace geometry::type_metaprogramming
