#pragma once

namespace geometry::detail {

template <class List, template <class> class trait>
struct type_for_each
{};

template <
    template <class...>
    class list,
    class... Args,
    template <class>
    class trait>
struct type_for_each<list<Args...>, trait>
{
  using type = list<typename trait<Args>::type...>;
};

template <class List, template <class> class trait>
using type_for_each_t = typename type_for_each<List, trait>::type;

}  // namespace geometry::detail
