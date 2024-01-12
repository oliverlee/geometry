#pragma once

#include "geometry/src/detail/contract_dimensions.hpp"
#include "geometry/src/detail/sort_dimensions.hpp"
#include "geometry/src/detail/strictly_increasing.hpp"

#include <cstddef>
#include <type_traits>

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

  /// blade
  /// @tparam Is dimensions
  ///
  /// Defines a blade for the given algebra.
  ///
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
  };

private:
  template <std::size_t... Is>
  static auto rebind(std::index_sequence<Is...>) -> blade<Is...>;

  template <class Seq>
  using rebind_t = decltype(rebind(Seq{}));

  template <std::size_t... Is>
  using reified_blade_t = rebind_t<detail::contract_dimensions_t<
      detail::contraction_map::projective,
      detail::sort_dimensions_t<std::index_sequence<Is...>>>>;

  template <std::size_t... Is>
  static constexpr auto reified_blade_coefficient_v =
      (detail::sort_dimensions_swap_count_v<std::index_sequence<Is...>> % 2 == 0
           ? scalar_type{1}
           : -scalar_type{1}) *
      scalar_type{detail::contract_dimensions_coefficient_v<
          detail::contraction_map::projective,
          detail::sort_dimensions_t<std::index_sequence<Is...>>>};

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
};

}  // namespace geometry
