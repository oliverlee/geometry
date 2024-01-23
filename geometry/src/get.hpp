#pragma once

#include "geometry/src/algebra_type.hpp"

#include <type_traits>
#include <utility>

namespace geometry {

/// access a blade of a multivector
///
template <
    class B,
    class V,
    class = std::enable_if_t<algebra_type_t<
        std::decay_t<V>>::template is_multivector_v<std::decay_t<V>>>>
constexpr auto get(V&& v)
{
  return get<B>(std::forward<V>(v));
}

}  // namespace geometry
