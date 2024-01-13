#pragma once

#include "geometry/src/detail/ordered_dimensions.hpp"
#include "geometry/type_metaprogramming.hpp"

#include <cstddef>
#include <utility>

namespace geometry::detail {

template <template <std::size_t...> class blade, std::size_t N>
struct grade
{
  template <std::size_t I, const auto& dims>
  struct as_blade
  {
    template <std::size_t... Js>
    static auto impl(std::index_sequence<Js...>) -> blade<dims[I][Js]...>;

    using type = decltype(impl(std::make_index_sequence<dims[I].size()>{}));
  };

  template <std::size_t I, const auto& dims>
  using as_blade_t = typename as_blade<I, dims>::type;

  template <class K>
  struct choose_k
  {
    static constexpr auto dimensions = ordered_dimensions<N, K::value>;

    template <std::size_t... Is>
    static auto impl(std::index_sequence<Is...>)
        -> tmp::list<as_blade_t<Is, dimensions>...>;

    template <std::size_t I>
    static auto impl(std::index_sequence<I>)
        -> std::enable_if_t<I == 0 and K::value == 0, tmp::list<blade<>>>;

    using type = decltype(impl(std::make_index_sequence<dimensions.size()>{}));
  };
};

template <template <std::size_t...> class blade>
struct blade_list
{
  static constexpr auto N = blade<>::algebra_type::dimension;

  using type = tmp::flatten_t<tmp::transform_t<
      tmp::make_index_constant_sequence<N + 1>,
      grade<blade, N>::template choose_k>>;
};

template <template <std::size_t...> class blade>
using blade_list_t = typename blade_list<blade>::type;

}  // namespace geometry::detail
