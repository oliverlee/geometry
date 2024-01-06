#pragma once

#include <algorithm>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace geometry::detail {

template <class InSeq, class OutSeq = std::index_sequence<>>
struct sort_swap_first
{
  using type = OutSeq;
};
template <std::size_t I0, std::size_t... Js>
struct sort_swap_first<std::index_sequence<I0>, std::index_sequence<Js...>>
{
  using type = std::index_sequence<Js..., I0>;
};
template <std::size_t I0, std::size_t I1, std::size_t... Is, std::size_t... Js>
struct sort_swap_first<
    std::index_sequence<I0, I1, Is...>,
    std::index_sequence<Js...>>
    : std::conditional_t<
          (I0 > I1),
          sort_swap_first<std::index_sequence<>,
                          std::index_sequence<Js..., I1, I0, Is...>>,
          sort_swap_first<std::index_sequence<I1, Is...>,
                          std::index_sequence<Js..., I0>>>
{};

template <class Seq>
using sort_swap_first_t = typename sort_swap_first<Seq>::type;

template <class Seq, std::size_t Swaps = 0, class = void>
struct sort_swap : sort_swap<sort_swap_first_t<Seq>, Swaps + 1>
{};
template <std::size_t... Is, std::size_t Swaps>
struct sort_swap<
    std::index_sequence<Is...>,
    Swaps,
    std::enable_if_t<std::is_same_v<
        std::index_sequence<Is...>,
        sort_swap_first_t<std::index_sequence<Is...>>>>>
{
  using type = std::index_sequence<Is...>;
  static constexpr auto swap_count = Swaps;
};

template <class Seq>
struct sort_dimensions
{
  using type = typename sort_swap<Seq>::type;
};

template <class Seq>
using sort_dimensions_t = typename sort_dimensions<Seq>::type;

template <class Seq>
struct sort_dimensions_swap_count
    : std::integral_constant<std::size_t, sort_swap<Seq>::swap_count>
{};

template <class Seq>
inline constexpr auto sort_dimensions_swap_count_v =
    sort_dimensions_swap_count<Seq>::value;

}  // namespace geometry::detail
