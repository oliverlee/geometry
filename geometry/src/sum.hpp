#pragma once

#include "geometry/src/common_algebra_type.hpp"
#include "geometry/src/detail/ordered.hpp"
#include "geometry/src/get.hpp"
#include "geometry/src/to_multivector.hpp"
#include "geometry/type_metaprogramming.hpp"

#include <tuple>

namespace geometry {

inline constexpr class
{
  template <template <class...> class multivector, class... B1s, class... B2s>
  static constexpr auto
  impl(const multivector<B1s...>& x, const multivector<B2s...>& y)
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

public:
  template <class T1, class T2, class A = common_algebra_type_t<T1, T2>>
  [[nodiscard]]
  constexpr auto
  operator()(const T1& x, const T2& y) const
  {
    return impl(to_multivector<A>(x), to_multivector<A>(y));
  }
} sum{};

}  // namespace geometry
