#pragma once

#include "geometry/src/detail/ordered.hpp"
#include "geometry/src/get.hpp"
#include "geometry/type_metaprogramming.hpp"

#include <tuple>

namespace geometry {

inline constexpr class
{
public:
  template <template <class...> class multivector, class... B1s, class... B2s>
  [[nodiscard]]
  constexpr auto
  operator()(const multivector<B1s...>& x, const multivector<B2s...>& y) const
  {
    using detail::ordered;

    using M = tmp::rebind_into_t<
        tmp::transform_t<
            tmp::unique_t<
                tmp::sort_t<tmp::list<ordered<B1s>..., ordered<B2s>...>>>,
            tmp::get_type>,
        multivector>;

    auto z = M{};

    std::ignore = ((get<B1s>(z) = get<B1s>(x), true) and ...);
    std::ignore =
        (((multivector<B1s...>::template contains<B2s>
               ? get<B2s>(z) += get<B2s>(y)
               : get<B2s>(z) = get<B2s>(y)),
          true) and
         ...);

    return z;
  }
} sum{};

}  // namespace geometry
