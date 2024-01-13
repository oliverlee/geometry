#pragma once

#include <type_traits>

namespace geometry::type_metaprogramming {

template <class...>
struct same : std::true_type
{};

template <class T, class... Ts>
struct same<T, Ts...> : std::bool_constant<((std::is_same_v<T, Ts>)and...)>
{};

template <class... Ts>
inline constexpr auto same_v = same<Ts...>::value;

}  // namespace geometry::type_metaprogramming
