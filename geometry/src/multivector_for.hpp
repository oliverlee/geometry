#pragma once

#include "geometry/src/algebra_type.hpp"

#include <type_traits>

namespace geometry {
namespace detail {

template <class A, class T, class = void>
struct multivector_for
{};

template <class A, template <class...> class multivector, class... Bs>
struct multivector_for<
    A,
    multivector<Bs...>,
    std::enable_if_t<std::is_same_v<A, algebra_type_t<multivector<Bs...>>>>>
{
  using type = typename A::template multivector<Bs...>;
};

template <class A, template <std::size_t...> class blade, std::size_t... Is>
struct multivector_for<
    A,
    blade<Is...>,
    std::enable_if_t<std::is_same_v<A, algebra_type_t<blade<Is...>>>>>
{
  using type = typename A::template multivector<blade<Is...>>;
};

template <class A, class T>
struct multivector_for<
    A,
    T,
    std::enable_if_t<std::is_convertible_v<T, typename A::scalar_type>>>
{
  using type = typename A::template multivector<typename A::template blade<>>;
};

}  // namespace detail

template <class A, class T>
struct multivector_for : detail::multivector_for<A, T>
{};

template <class A, class T>
using multivector_for_t = typename multivector_for<A, T>::type;

}  // namespace geometry
