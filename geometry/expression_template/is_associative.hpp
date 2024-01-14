#pragma once

#include <type_traits>

namespace geometry::expression_template {

template <class T, class = void>
struct is_associative : std::false_type
{};

template <class T>
struct is_associative<T, std::void_t<decltype(T::is_associative)>>
    : std::bool_constant<T::is_associative>
{};

template <class T>
inline constexpr auto is_associative_v = is_associative<T>::value;

}  // namespace geometry::expression_template
