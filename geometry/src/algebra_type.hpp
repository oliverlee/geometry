#pragma once

#include <type_traits>

namespace geometry {
namespace detail {

template <class T, class = void>
struct algebra_type
{};

template <class T>
struct algebra_type<T, std::void_t<typename T::algebra_type>>
{
  using type = typename T::algebra_type;
};

}  // namespace detail

template <class T>
struct algebra_type : detail::algebra_type<T>
{};

template <class T>
using algebra_type_t = typename algebra_type<T>::type;

}  // namespace geometry
