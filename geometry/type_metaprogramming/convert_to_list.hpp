#pragma once

#include <cstddef>
#include <utility>

namespace geometry::type_metaprogramming {

template <class Seq, template <class...> class list>
struct convert_to_list;

template <
    std::size_t... Is,
    template <std::size_t...>
    class seq,
    template <class...>
    class list>
struct convert_to_list<seq<Is...>, list>
{
  using type = list<std::integral_constant<std::size_t, Is>...>;
};

template <std::size_t... Is, template <class...> class list>
struct convert_to_list<std::index_sequence<Is...>, list>
{
  using type = list<std::integral_constant<std::size_t, Is>...>;
};

template <class Seq, template <class...> class list>
using convert_to_list_t = typename convert_to_list<Seq, list>::type;

}  // namespace geometry::type_metaprogramming
