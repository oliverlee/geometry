#pragma once

#include <utility>

namespace geometry {

/// access a blade of a multivector
///
template <class B, class V>
constexpr decltype(auto) get(V&& v)
{
  return get<B>(std::forward<V>(v));
}

}  // namespace geometry
