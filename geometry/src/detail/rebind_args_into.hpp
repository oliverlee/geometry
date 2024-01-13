#pragma once

namespace geometry::detail {

template <class From, template <class...> class to>
struct rebind_args_into
{};
template <
    class... Args,
    template <class...>
    class from,
    template <class...>
    class to>
struct rebind_args_into<from<Args...>, to>
{
  using type = to<Args...>;
};

template <class From, template <class...> class to>
using rebind_args_into_t = typename rebind_args_into<From, to>::type;

}  // namespace geometry::detail
