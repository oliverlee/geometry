#pragma once

#include <type_traits>

namespace geometry::detail {

template <class...>
struct all_same : std::false_type
{};

template <>
struct all_same<> : std::true_type
{};
template <class T>
struct all_same<T> : std::true_type
{};
template <class T, class... Ts>
struct all_same<T, Ts...> : std::bool_constant<((std::is_same_v<T, Ts>)and...)>
{};

template <class... Ts>
inline constexpr auto all_same_v = all_same<Ts...>::value;

}  // namespace geometry::detail
