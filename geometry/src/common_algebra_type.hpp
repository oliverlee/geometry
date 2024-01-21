#pragma once

#include "geometry/src/algebra_type.hpp"

#include <type_traits>

namespace geometry {
namespace detail {

template <class T1, class T2, class = void>
struct common_algebra_type
{};

template <class T1, class T2>
struct common_algebra_type<
    T1,
    T2,
    std::void_t<std::common_type_t<algebra_type_t<T1>, algebra_type_t<T2>>>>
    : std::common_type<algebra_type_t<T1>, algebra_type_t<T2>>
{};

template <class T1, class T2>
struct common_algebra_type<
    T1,
    T2,
    std::enable_if_t<
        std::is_convertible_v<T1, typename algebra_type_t<T2>::scalar_type>>>
    : algebra_type<T2>
{};

template <class T1, class T2>
struct common_algebra_type<
    T1,
    T2,
    std::enable_if_t<
        std::is_convertible_v<T2, typename algebra_type_t<T1>::scalar_type>>>
    : algebra_type<T1>
{};

}  // namespace detail

template <class T1, class T2>
struct common_algebra_type : detail::common_algebra_type<T1, T2>
{};

template <class T1, class T2>
using common_algebra_type_t = typename common_algebra_type<T1, T2>::type;

}  // namespace geometry
