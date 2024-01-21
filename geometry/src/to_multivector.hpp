#pragma once

#include "geometry/src/detail/construct_if_convertible.hpp"
#include "geometry/src/multivector_for.hpp"

#include <type_traits>

namespace geometry {
namespace detail {

template <class Algebra>
class to_multivector_fn
{
public:
  template <
      class T,
      class R = multivector_for_t<Algebra, T>,
      std::enable_if_t<std::is_same_v<T, R>, bool> = true>
  constexpr auto& operator()(const T& value) const
  {
    return value;
  }
  template <
      class T,
      class R = multivector_for_t<Algebra, T>,
      std::enable_if_t<not std::is_same_v<T, R>, bool> = true>
  constexpr auto operator()(const T& value) const
  {
    using S = typename Algebra::scalar_type;
    return R{detail::construct_if_convertible<S>(value)};
  }
};

}  // namespace detail

template <class Algebra>
inline constexpr auto to_multivector = detail::to_multivector_fn<Algebra>{};

}  // namespace geometry
