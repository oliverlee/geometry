#pragma once

#include "geometry/src/detail/type_list.hpp"

namespace geometry::detail {

template <class InSeq, class OutSeq = type_list<>>
struct type_unique
{
  using type = OutSeq;
};
template <class T, class... Rs>
struct type_unique<type_list<T>, type_list<Rs...>>
    : type_unique<type_list<>, type_list<Rs..., T>>
{};
template <class T, class... Ts, class... Rs>
struct type_unique<type_list<T, T, Ts...>, type_list<Rs...>>
    : type_unique<type_list<T, Ts...>, type_list<Rs...>>
{};
template <class T0, class T1, class... Ts, class... Rs>
struct type_unique<type_list<T0, T1, Ts...>, type_list<Rs...>>
    : type_unique<type_list<T1, Ts...>, type_list<Rs..., T0>>
{};

template <class Seq>
using type_unique_t = typename type_unique<Seq>::type;

}  // namespace geometry::detail
