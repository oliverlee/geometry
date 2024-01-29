#pragma once

#include "geometry/src/algebra_type.hpp"

#include <type_traits>
#include <utility>

namespace geometry {
namespace detail {

template <class T>
auto get() -> void = delete;

template <class B>
struct get_fn
{
  template <
      class V,
      class = std::enable_if_t<
          algebra_type_t<std::decay_t<V>>::template is_multivector_v<
              std::decay_t<V>> and
          std::decay_t<V>::template contains<B>>>
  constexpr decltype(auto) operator()(V && v) const
  {
    return get<B>(std::forward<V>(v));
  }
};

}  // namespace detail

inline namespace cpo {

/// access a blade of a multivector
///
template <class B>
inline constexpr auto get = detail::get_fn<B>{};

}  // namespace cpo
}  // namespace geometry
