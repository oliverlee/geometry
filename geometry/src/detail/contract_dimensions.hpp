#pragma once

#include <cstddef>
#include <utility>

namespace geometry::detail {

struct contraction_map
{
  struct projective
  {
    template <std::size_t I>
    static constexpr auto value = static_cast<int>(I != 0);
  };
};

template <
    class Map,
    class InSeq,
    int Coeff = 1,
    class OutSeq = std::index_sequence<>>
struct contract_dimensions
{
  using type = OutSeq;
  static constexpr auto coefficient = Coeff;
};
template <
    class Map,
    std::size_t I0,
    std::size_t... Is,
    int Coeff,
    std::size_t... Js>
struct contract_dimensions<
    Map,
    std::index_sequence<I0, Is...>,
    Coeff,
    std::index_sequence<Js...>>
    : contract_dimensions<
          Map,
          std::index_sequence<Is...>,
          Coeff,
          std::index_sequence<Js..., I0>>
{};
template <
    class Map,
    std::size_t I0,
    std::size_t... Is,
    int Coeff,
    std::size_t... Js>
struct contract_dimensions<
    Map,
    std::index_sequence<I0, I0, Is...>,
    Coeff,
    std::index_sequence<Js...>>
    : contract_dimensions<
          Map,
          std::index_sequence<Is...>,
          Coeff * Map::template value<I0>,
          std::index_sequence<Js...>>
{};

template <class Map, class Seq>
using contract_dimensions_t = typename contract_dimensions<Map, Seq>::type;

template <class Map, class Seq>
struct contract_dimensions_coefficient
    : std::integral_constant<int, contract_dimensions<Map, Seq>::coefficient>
{};

template <class Map, class Seq>
inline constexpr auto contract_dimensions_coefficient_v =
    contract_dimensions_coefficient<Map, Seq>::value;

}  // namespace geometry::detail
