#pragma once

#include "geometry/src/detail/type_list.hpp"

#include <cstddef>
#include <type_traits>
#include <utility>

namespace geometry::detail {

template <class InSeq, class OutSeq = type_list<>>
struct sort_swap_first
{
  using type = OutSeq;
};

template <class I0, class... Js>
struct sort_swap_first<type_list<I0>, type_list<Js...>>
{
  using type = type_list<Js..., I0>;
};
template <class I0, class I1, class... Is, class... Js>
struct sort_swap_first<type_list<I0, I1, Is...>, type_list<Js...>>
    : std::conditional_t<
          (I0{} > I1{}),
          sort_swap_first<type_list<>, type_list<Js..., I1, I0, Is...>>,
          sort_swap_first<type_list<I1, Is...>, type_list<Js..., I0>>>
{};

template <class Seq>
using sort_swap_first_t = typename sort_swap_first<Seq>::type;

template <class Seq, std::size_t Swaps = 0, class = void>
struct sort_swap : sort_swap<sort_swap_first_t<Seq>, Swaps + 1>
{};
template <class... Is, std::size_t Swaps>
struct sort_swap<
    type_list<Is...>,
    Swaps,
    std::enable_if_t<
        std::is_same_v<type_list<Is...>, sort_swap_first_t<type_list<Is...>>>>>
{
  using type = type_list<Is...>;
  static constexpr auto swap_count = Swaps;
};

template <class Seq>
struct type_sort
{
  using type = typename sort_swap<Seq>::type;
};

template <class Seq>
using type_sort_t = typename type_sort<Seq>::type;

template <class Seq>
struct type_sort_swap_count
    : std::integral_constant<std::size_t, sort_swap<Seq>::swap_count>
{};

template <class Seq>
inline constexpr auto type_sort_swap_count_v = type_sort_swap_count<Seq>::value;

}  // namespace geometry::detail
