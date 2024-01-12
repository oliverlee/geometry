#pragma once

#include "geometry/src/detail/all_same.hpp"
#include "geometry/src/detail/blade_list.hpp"
#include "geometry/src/detail/contract_dimensions.hpp"
#include "geometry/src/detail/get_type.hpp"
#include "geometry/src/detail/ordered.hpp"
#include "geometry/src/detail/rebind_args_into.hpp"
#include "geometry/src/detail/strictly_increasing.hpp"
#include "geometry/src/detail/type_for_each.hpp"
#include "geometry/src/detail/type_sort.hpp"
#include "geometry/src/detail/type_unique.hpp"
#include "geometry/src/type_list.hpp"

#include <cstddef>
#include <ostream>
#include <tuple>
#include <type_traits>
#include <utility>

namespace geometry {

/// access a blade of a multivector
///
template <class B, class V>
constexpr decltype(auto) get(V&& v)
{
  return get<B>(std::forward<V>(v));
}

/// represents a projective geometric algebra
/// @tparam S scalar type
/// @tparam N number of euclidean geometry dimensions
///
/// Models an N-dimensional projective geometric algebra denoted as
/// @f[
/// P(S^*_{N,0,1})
/// @f]
///
template <class S, std::size_t N>
struct algebra
{
  /// scalar type
  ///
  using scalar_type = S;

  /// algebra dimension
  ///
  static constexpr auto dimension = N + 1;

  /// blade
  /// @tparam Is dimensions
  ///
  /// Defines a blade for the given algebra.
  ///
  template <std::size_t... Is>
  struct blade;

  /// multivector
  /// @tparam Bs blade types
  ///
  /// A linear combination of blades.
  ///
  /// @note a multivector may be composed of only blades with the same grade
  ///
  template <class... Bs>
  struct multivector;

  /// determines if a type is a blade
  /// @tparam T type
  ///
  /// @{
  template <class T>
  struct is_blade : std::false_type
  {};
  template <std::size_t... Is>
  struct is_blade<blade<Is...>> : std::true_type
  {};
  template <class T>
  static constexpr auto is_blade_v = is_blade<T>::value;
  /// @}

  /// list of blades in the algebra
  ///
  using blade_list_type = detail::blade_list_t<blade>;

private:
  template <class... Is>
  static auto rebind(type_list<Is...>) -> blade<Is::value...>;

  template <class Seq>
  using rebind_t = decltype(rebind(Seq{}));

  template <std::size_t... Is>
  using sequence_t = type_list<std::integral_constant<std::size_t, Is>...>;

  template <std::size_t... Is>
  using reified_blade_t = rebind_t<detail::contract_dimensions_t<
      detail::contraction_map::projective,
      detail::type_sort_t<sequence_t<Is...>>>>;

  template <std::size_t... Is>
  static constexpr auto reified_blade_coefficient_v =
      (detail::type_sort_swap_count_v<sequence_t<Is...>> % 2 == 0
           ? scalar_type{1}
           : -scalar_type{1}) *
      scalar_type{detail::contract_dimensions_coefficient_v<
          detail::contraction_map::projective,
          detail::type_sort_t<sequence_t<Is...>>>};

public:
  /// unit blade
  /// @tparam Is dimensions
  ///
  /// Obtains the unit blade corresponding to dimensions `Is...` for the given
  /// algebra. `Is...` will be reified if not provided in canonical form (i.e.
  /// in non-decreasing order).
  ///
  /// ~~~{.cpp}
  /// static_assert(e<1, 2> == -e<2, 1>);
  /// ~~~
  ///
  /// A unit blade specified with zero dimensions (i.e. `e<>`) is semantically
  /// equivalent to a scalar.
  ///
  /// ~~~{.cpp}
  /// static_assert(2 == 2*e<>);
  /// ~~~
  ///
  /// Squared basis vectors in a blade (i.e. `e<i, i>`) will contract to `0` for
  /// `i == 0` and `1` for `i != 0`.
  ///
  template <std::size_t... Is>
  static constexpr auto e =
      reified_blade_t<Is...>{reified_blade_coefficient_v<Is...>};

  template <std::size_t... Is>
  struct blade
  {
    static_assert(sizeof...(Is) <= N + 1, "number of `Is` exceeds rank");
    static_assert(((Is <= N) and ...), "`Is` value exceeds rank");
    static_assert(
        detail::strictly_increasing(Is...),
        "`Is` values must be unique and sorted");

    /// algebra type
    ///
    using algebra_type = algebra;

    /// scalar type
    ///
    using scalar_type = typename algebra::scalar_type;

    /// grade
    ///
    static constexpr auto grade = sizeof...(Is);

    /// k-vector coefficient
    ///
    scalar_type coefficient{};

    /// construct a zero blade
    ///
    blade() = default;

    /// construct a blade
    /// @param a coefficient
    /// @note this constructor is explicit if and only if grade != 0
    ///
    /// @{
    template <std::size_t M = grade, std::enable_if_t<M == 0, bool> = true>
    constexpr blade(scalar_type a) : coefficient{a}
    {}
    template <std::size_t M = grade, std::enable_if_t<M != 0, bool> = true>
    constexpr explicit blade(scalar_type a) : coefficient{a}
    {}
    /// @}

    /// comparison operators
    ///
    /// @{
    [[nodiscard]]
    friend constexpr auto
    operator==(blade x, blade y) noexcept -> bool
    {
      return x.coefficient == y.coefficient;
    }
    [[nodiscard]]
    friend constexpr auto
    operator!=(blade x, blade y) noexcept -> bool
    {
      return x.coefficient != y.coefficient;
    }
    [[nodiscard]]
    friend constexpr auto
    operator<(blade x, blade y) noexcept -> bool
    {
      return x.coefficient < y.coefficient;
    }
    [[nodiscard]]
    friend constexpr auto
    operator>(blade x, blade y) noexcept -> bool
    {
      return x.coefficient > y.coefficient;
    }
    [[nodiscard]]
    friend constexpr auto
    operator<=(blade x, blade y) noexcept -> bool
    {
      return x.coefficient <= y.coefficient;
    }
    [[nodiscard]]
    friend constexpr auto
    operator>=(blade x, blade y) noexcept -> bool
    {
      return x.coefficient >= y.coefficient;
    }
    /// @}

    /// unary negation
    ///
    [[nodiscard]]
    friend constexpr auto
    operator-(blade x) -> blade
    {
      return blade{-x.coefficient};
    }

    /// addition
    ///
    /// @{
    [[nodiscard]]
    friend constexpr auto
    operator+(blade x, blade y) -> blade
    {
      return blade{x.coefficient + y.coefficient};
    }
    template <std::size_t... Js>
    [[nodiscard]]
    friend constexpr auto
    operator+(blade x, blade<Js...> y)
    {
      if constexpr (
          detail::ordered<blade<Js...>>{} < detail::ordered<blade>{}) {
        return multivector<blade<Js...>, blade>{y, x};
      } else {
        return multivector<blade, blade<Js...>>{x, y};
      }
    }
    friend constexpr auto& operator+=(blade& x, blade y) { return x = x + y; }
    /// @}

    /// subtraction
    ///
    /// @{
    template <std::size_t... Js>
    [[nodiscard]]
    friend constexpr auto
    operator-(blade x, blade<Js...> y)
    {
      return x + -y;
    }
    friend constexpr auto& operator-=(blade& x, blade y) { return x = x - y; }
    /// @}

    /// scalar multiplication
    ///
    /// @{
    [[nodiscard]]
    friend constexpr auto
    operator*(scalar_type a, blade x) -> blade
    {
      return blade{a * x.coefficient};
    }
    [[nodiscard]]
    friend constexpr auto
    operator*(blade x, scalar_type a) -> blade
    {
      return a * x;
    }
    /// @}

    /// geometric product
    ///
    template <std::size_t... Js>
    [[nodiscard]]
    friend constexpr auto
    operator*(blade x, blade<Js...> y)
    {
      return x.coefficient * y.coefficient * e<Is..., Js...>;
    }

    /// stream insertion
    ///
    friend auto operator<<(std::ostream& os, blade x) -> std::enable_if_t<
        (N < 10),
        decltype(os << std::declval<const scalar_type&>(), os)>
    {
      if (sizeof...(Is) == 0) {
        os << x.coefficient;
        return os;
      }

      if (x.coefficient != scalar_type{1}) {
        os << x.coefficient << "*";
      }

      os << "e";
      std::ignore = ((os << Is, true) and ...);

      return os;
    }
  };

  template <class... Bs>
  struct multivector : Bs...
  {
    static_assert(
        (is_blade_v<Bs> and ...), "`Bs` must be a specialization of blade");
    static_assert(
        detail::all_same_v<typename Bs::scalar_type...>,
        "`Bs` must have the same scalar type");
    static_assert(
        detail::strictly_increasing(detail::ordered<Bs>{}...),
        "`Bs` must be lexicographically sorted");

    /// algebra type
    ///
    using algebra_type = algebra;

    /// determine if a blade is contained in this multivector
    ///
    template <class B>
    static constexpr auto contains = (std::is_same_v<B, Bs> or ...);

    /// construct a zero multivector
    ///
    multivector() = default;

    /// construct a multivector from blades
    ///
    constexpr explicit multivector(Bs... bs) : Bs{bs}... {}

    /// access a specific blade
    ///
    /// @{
    template <class B>
    friend constexpr auto
    get(multivector& x) -> std::enable_if_t<contains<B>, B&>
    {
      return static_cast<B&>(x);
    }
    template <class B>
    friend constexpr auto
    get(const multivector& x) -> std::enable_if_t<contains<B>, const B&>
    {
      return static_cast<const B&>(x);
    }
    template <class B>
    friend constexpr auto
    // NOLINTNEXTLINE(cppcoreguidelines-rvalue-reference-param-not-moved)
    get(multivector&& x) -> std::enable_if_t<contains<B>, B&&>
    {
      return static_cast<B&&>(x);
    }
    template <class B>
    friend constexpr auto
    get(const multivector&& x) -> std::enable_if_t<contains<B>, const B&&>
    {
      return static_cast<const B&&>(x);
    }
    /// @}

    /// access a specific blade or return a default value
    ///
    /// @{
    template <class B>
    friend constexpr auto
    get_or(const multivector& x, const B& value) -> const B&
    {
      if constexpr (contains<B>) {
        return get<B>(x);
      } else {
        return value;
      }
    }
    /// @}

    /// equality comparison
    ///
    /// @{
    template <class... B2s>
    [[nodiscard]]
    friend constexpr auto
    operator==(const multivector& x, const multivector<B2s...>& y) -> bool
    {
      return ((get_or(x, Bs{}) == get_or(y, Bs{})) and ...) and
             ((get_or(x, B2s{}) == get_or(y, B2s{})) and ...);
    }
    template <class... B2s>
    [[nodiscard]]
    friend constexpr auto
    operator!=(const multivector& x, const multivector<B2s...>& y) -> bool
    {
      return not(x == y);
    }
    template <std::size_t... Is>
    [[nodiscard]]
    friend constexpr auto
    operator==(const multivector& x, blade<Is...> y) -> bool
    {
      return x == multivector<blade<Is...>>{y};
    }
    template <std::size_t... Is>
    [[nodiscard]]
    friend constexpr auto
    operator==(blade<Is...> x, const multivector& y) -> bool
    {
      return multivector<blade<Is...>>{x} == y;
    }
    template <std::size_t... Is>
    [[nodiscard]]
    friend constexpr auto
    operator!=(const multivector& x, blade<Is...> y) -> bool
    {
      return not(x == y);
    }
    template <std::size_t... Is>
    [[nodiscard]]
    friend constexpr auto
    operator!=(blade<Is...> x, const multivector& y) -> bool
    {
      return not(x == y);
    }

    /// @}

    /// unary negation
    ///
    [[nodiscard]]
    friend constexpr auto
    operator-(const multivector& x) -> multivector
    {
      return multivector{-static_cast<Bs>(x)...};
    }

    /// addition
    ///
    /// @{
    template <std::size_t... Is>
    [[nodiscard]]
    friend constexpr auto
    operator+(const multivector& x, blade<Is...> y)
    {
      return x + multivector<blade<Is...>>{y};
    }
    template <std::size_t... Is>
    [[nodiscard]]
    friend constexpr auto
    operator+(blade<Is...> x, const multivector& y)
    {
      return y + x;
    }
    template <class... B2s>
    [[nodiscard]]
    friend constexpr auto
    operator+(const multivector& x, const multivector<B2s...>& y)
    {
      using detail::get_type;
      using detail::ordered;
      using detail::rebind_args_into_t;
      using detail::type_for_each_t;
      using detail::type_sort_t;
      using detail::type_unique_t;

      using M = rebind_args_into_t<
          type_for_each_t<
              type_unique_t<
                  type_sort_t<type_list<ordered<Bs>..., ordered<B2s>...>>>,
              get_type>,
          multivector>;

      auto z = M{};

      std::ignore = ((get<Bs>(z) = get<Bs>(x), true) and ...);
      std::ignore = ((get<B2s>(z) += get<B2s>(y), true) and ...);

      return z;
    }
    /// @}

    /// subtraction
    ///
    /// @{
    template <std::size_t... Is>
    [[nodiscard]]
    friend constexpr auto
    operator-(const multivector& x, blade<Is...> y)
    {
      return x + -y;
    }
    template <std::size_t... Is>
    [[nodiscard]]
    friend constexpr auto
    operator-(blade<Is...> x, const multivector& y)
    {
      return x + -y;
    }
    template <class... B2s>
    [[nodiscard]]
    friend constexpr auto
    operator-(const multivector& x, const multivector<B2s...>& y)
        -> decltype(x + y)
    {
      return x + -y;
    }
    /// @}

    /// geometric product
    ///
    /// @{
    template <std::size_t... Is>
    [[nodiscard]]
    friend constexpr auto
    operator*(const multivector& x, blade<Is...> y)
    {
      return ((get<Bs>(x) * y) + ...);
    }
    template <std::size_t... Is>
    [[nodiscard]]
    friend constexpr auto
    operator*(blade<Is...> x, const multivector& y)
    {
      return -y * x;
    }
    template <class... B2s>
    [[nodiscard]]
    friend constexpr auto
    operator*(const multivector& x, const multivector<B2s...>& y)
    {
      return ((x * get<B2s>(y)) + ...);
    }
    /// @}

    /// stream insertion
    ///
    friend auto operator<<(std::ostream& os, const multivector& x)
        -> decltype(((os << std::declval<const Bs&>(), true) and ...), os)
    {
      auto first = true;

      std::ignore =
          ((os << (std::exchange(first, false) ? "" : " + "),
            os << get<Bs>(x),
            true) and
           ...);

      return os;
    }
  };
};

}  // namespace geometry
