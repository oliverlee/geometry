
namespace geometry::type_metaprogramming {

template <class L, template <class> class func>
struct transform;

template <
    template <class...>
    class list,
    class... Ts,
    template <class>
    class func>
struct transform<list<Ts...>, func>
{
  using type = list<typename func<Ts>::type...>;
};

template <class L, template <class> class func>
using transform_t = typename transform<L, func>::type;

}  // namespace geometry::type_metaprogramming
