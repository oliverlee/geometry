#pragma once

#include "geometry/src/algebra_type.hpp"

#include <type_traits>

namespace geometry {
namespace detail {

template <class T, class U, class = void>
struct common_algebra_type
{};

template <class T, class U>
struct common_algebra_type<
    T,
    U,
    std::void_t<std::common_type_t<algebra_type_t<T>, algebra_type_t<U>>>>
    : std::common_type<algebra_type_t<T>, algebra_type_t<U>>
{};

template <class T, class U>
struct common_algebra_type<
    T,
    U,
    std::enable_if_t<
        std::is_convertible_v<T, typename algebra_type_t<U>::scalar_type>>>
    : algebra_type<U>
{};

template <class T, class U>
struct common_algebra_type<
    T,
    U,
    std::enable_if_t<
        std::is_convertible_v<U, typename algebra_type_t<T>::scalar_type>>>
    : algebra_type<T>
{};

}  // namespace detail

template <class T, class U>
struct common_algebra_type : detail::common_algebra_type<T, U>
{};

template <class T, class U>
using common_algebra_type_t = typename common_algebra_type<T, U>::type;

}  // namespace geometry
