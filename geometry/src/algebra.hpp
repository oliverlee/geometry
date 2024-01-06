#pragma once

#include <cstddef>
#include <type_traits>

namespace geometry {

template <class S, std::size_t N>
struct algebra
{
  using scalar_type = S;

  template <std::size_t... Is>
  struct blade
  {
    static_assert(sizeof...(Is) <= N + 1, "number of `Is` exceeds rank");
    static_assert(((Is <= N) and ...), "`Is` exceeds rank");
    // TODO check Is.. values are unique

    using algebra_type = algebra;
    using scalar_type = typename algebra::scalar_type;

    static constexpr auto grade = sizeof...(Is);

    scalar_type coefficient{};

    blade() = default;

    template <std::size_t M = grade, std::enable_if_t<M == 0, bool> = true>
    constexpr blade(scalar_type a) : coefficient{a}
    {}
    template <std::size_t M = grade, std::enable_if_t<M != 0, bool> = true>
    constexpr explicit blade(scalar_type a) : coefficient{a}
    {}

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

    [[nodiscard]]
    friend constexpr auto
    operator-(blade x) -> blade
    {
      return blade{-x.coefficient};
    }

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
  };

  template <std::size_t... Is>
  static constexpr auto e = blade<Is...>{scalar_type{1}};
};

}  // namespace geometry
