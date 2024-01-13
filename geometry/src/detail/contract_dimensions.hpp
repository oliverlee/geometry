#pragma once

#include "geometry/type_metaprogramming.hpp"

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
    class OutSeq = tmp::empty_t<InSeq>>
struct contract_dimensions
{
  using type = OutSeq;
  static constexpr auto coefficient = Coeff;
};
template <
    template <class...>
    class list,
    class Map,
    class I0,
    class... Is,
    int Coeff,
    class... Js>
struct contract_dimensions<Map, list<I0, Is...>, Coeff, list<Js...>>
    : contract_dimensions<Map, list<Is...>, Coeff, list<Js..., I0>>
{};
template <
    template <class...>
    class list,
    class Map,
    class I0,
    class... Is,
    int Coeff,
    class... Js>
struct contract_dimensions<Map, list<I0, I0, Is...>, Coeff, list<Js...>>
    : contract_dimensions<
          Map,
          list<Is...>,
          Coeff * Map::template value<I0{}>,
          list<Js...>>
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
