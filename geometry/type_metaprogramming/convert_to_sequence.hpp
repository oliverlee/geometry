#pragma once

#include <cstddef>

namespace geometry::type_metaprogramming {

template <class List, template <std::size_t...> class seq>
struct convert_to_sequence;

template <
    class... Is,
    template <class...>
    class list,
    template <std::size_t...>
    class seq>
struct convert_to_sequence<list<Is...>, seq>
{
  using type = seq<Is::value...>;
};

template <class List, template <std::size_t...> class seq>
using convert_to_sequence_t = typename convert_to_sequence<List, seq>::type;

}  // namespace geometry::type_metaprogramming
