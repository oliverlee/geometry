#pragma once

#include "geometry/expression_template.hpp"
#include "geometry/src/common_algebra_type.hpp"
#include "geometry/src/detail/blade_list.hpp"
#include "geometry/src/detail/construct_if_convertible.hpp"
#include "geometry/src/detail/contract_dimensions.hpp"
#include "geometry/src/detail/ordered.hpp"
#include "geometry/src/detail/strictly_increasing.hpp"
#include "geometry/src/geometric_product.hpp"
#include "geometry/src/get.hpp"
#include "geometry/src/multivector_for.hpp"
#include "geometry/src/sum.hpp"
#include "geometry/type_metaprogramming.hpp"

#include <cstddef>
#include <ostream>
#include <tuple>
#include <type_traits>
#include <utility>

namespace geometry {

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
  template <std::size_t... Is>
  using reified_blade_t = tmp::convert_to_sequence_t<
      detail::contract_dimensions_t<
          detail::contraction_map::projective,
          tmp::sort_t<tmp::index_constant_list<Is...>>>,
      blade>;

public:
  /// coefficient for blade product
  ///
  /// @{
  template <std::size_t... Is>
  struct reified_blade_coefficient
      : std::integral_constant<
            int,
            (tmp::sort_swap_count_v<tmp::index_constant_list<Is...>> % 2 == 0
                 ? 1
                 : -1) *
                detail::contract_dimensions_coefficient_v<
                    detail::contraction_map::projective,
                    tmp::sort_t<tmp::index_constant_list<Is...>>>>
  {};

  template <std::size_t... Is>
  static constexpr auto reified_blade_coefficient_v =
      reified_blade_coefficient<Is...>::value;
  /// @}

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
  /// Squared basis vectors in a blade (i.e. `e<i, i>`) will contract to `0`
  /// for `i == 0` and `1` for `i != 0`.
  ///
  template <std::size_t... Is>
  static constexpr auto e =
      reified_blade_t<Is...>{scalar_type{reified_blade_coefficient_v<Is...>}};

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
    /// @}

    /// compound assignment operators
    ///
    /// @{
    friend constexpr auto& operator+=(blade& x, blade y) { return x = x + y; }
    friend constexpr auto& operator-=(blade& x, blade y) { return x = x - y; }
    /// @}

    /// geometric product
    ///
    template <std::size_t... Js>
    [[nodiscard]]
    friend constexpr auto
    operator*(blade x, blade<Js...> y)
    {
      constexpr auto unit_blade_coeff =
          reified_blade_coefficient_v<Is..., Js...>;
      using B = reified_blade_t<Is..., Js...>;

      if constexpr (unit_blade_coeff == 0) {
        return B{};
      } else {
        const auto coeff = x.coefficient * y.coefficient;
        return B{unit_blade_coeff == 1 ? coeff : -coeff};
      }
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
        tmp::same_v<typename Bs::scalar_type...>,
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

    /// construct a multivector from a scalar
    ///
    template <
        class T = multivector<blade<>>,
        class = std::enable_if_t<std::is_same_v<T, multivector>>>
    constexpr explicit multivector(scalar_type s) : multivector(blade<>{s})
    {}

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
      return multivector{-get<Bs>(x)...};
    }

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

/// addition
///
template <class T, class U, class A = common_algebra_type_t<T, U>>
[[nodiscard]]
constexpr auto
operator+(const T& t, const U& u)
{
  using S = typename A::scalar_type;
  return sum(
      multivector_for_t<A, T>{detail::construct_if_convertible<S>(t)},
      multivector_for_t<A, U>{detail::construct_if_convertible<S>(u)});
}

/// subtraction
///
template <class T, class U, class A = common_algebra_type_t<T, U>>
[[nodiscard]]
constexpr auto
operator-(const T& t, const U& u)
{
  return t + -u;
}

/// geometric product
///
template <class T, class U, class A = common_algebra_type_t<T, U>>
[[nodiscard]]
constexpr auto
operator*(const T& t, const U& u)
{
  using S = typename A::scalar_type;
  return geometric_product(
      multivector_for_t<A, T>{detail::construct_if_convertible<S>(t)},
      multivector_for_t<A, U>{detail::construct_if_convertible<S>(u)});
}

}  // namespace geometry
