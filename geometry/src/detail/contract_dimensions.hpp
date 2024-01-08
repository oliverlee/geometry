#pragma once

#include "geometry/src/detail/type_list.hpp"

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

template <class Map, class InSeq, int Coeff = 1, class OutSeq = type_list<>>
struct contract_dimensions
{
  using type = OutSeq;
  static constexpr auto coefficient = Coeff;
};
template <class Map, class I0, class... Is, int Coeff, class... Js>
struct contract_dimensions<Map, type_list<I0, Is...>, Coeff, type_list<Js...>>
    : contract_dimensions<Map, type_list<Is...>, Coeff, type_list<Js..., I0>>
{};
template <class Map, class I0, class... Is, int Coeff, class... Js>
struct contract_dimensions<
    Map,
    type_list<I0, I0, Is...>,
    Coeff,
    type_list<Js...>>
    : contract_dimensions<
          Map,
          type_list<Is...>,
          Coeff * Map::template value<I0{}>,
          type_list<Js...>>
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
