#include "geometry/geometry.hpp"
#include "skytest/skytest.hpp"

#include "test/test_same.hpp"

#include <cstddef>
#include <ostream>
#include <type_traits>

template <class T>
struct test
{
  using algebra_type = ::geometry::algebra<T, 2>;

  template <std::size_t... Is>
  static constexpr auto e = algebra_type::template e<Is...>;

  template <std::size_t... Is>
  using blade = typename algebra_type::template blade<Is...>;

  template <class... Bs>
  using multivector = typename algebra_type::template multivector<Bs...>;
};

template <class Tag>
struct counted_double
{
  double value{};

  counted_double() = default;

  template <class T, class = std::enable_if_t<std::is_convertible_v<T, double>>>
  constexpr counted_double(T value) : value{static_cast<double>(value)}
  {}

  // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
  static int plus_count;
  static int multiplies_count;
  // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

  friend auto operator-(counted_double x) -> counted_double
  {
    return {-x.value};
  }
  friend auto operator+(counted_double x, counted_double y) -> counted_double
  {
    ++counted_double::plus_count;
    return {x.value + y.value};
  }
  friend auto operator*(counted_double x, counted_double y) -> counted_double
  {
    ++counted_double::multiplies_count;
    return {x.value * y.value};
  }
  friend auto operator==(counted_double x, counted_double y)
  {
    return x.value == y.value;
  }
  friend auto operator!=(counted_double x, counted_double y)
  {
    return x.value != y.value;
  }

  friend auto& operator<<(std::ostream& os, counted_double d)
  {
    os << d.value;
    return os;
  }
};

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
template <class Tag>
int counted_double<Tag>::plus_count = int{};
template <class Tag>
int counted_double<Tag>::multiplies_count = int{};
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

auto main() -> int
{
  using namespace ::skytest::literals;
  using ::geometry::test::same;
  using ::skytest::eq;
  using ::skytest::expect;

  "null generators pruned (0 blades)"_test = [] {
    struct tag
    {};
    using T = counted_double<tag>;
    using test = ::test<T>;

    const auto c = 2;

    const auto x = c * test::e<0> + test::e<0, 1>;
    const auto y = test::e<0>;

    T::plus_count = 0;
    T::multiplies_count = 0;

    const auto r = x * y;

    return expect(
        same(test::blade<>{}, r) and eq(0, T::plus_count) and
        eq(0, T::multiplies_count));
  };

  "null generators pruned (1 blade)"_test = [] {
    struct tag
    {};
    using T = counted_double<tag>;
    using test = ::test<T>;

    const auto a = 3;
    const auto c = 2;

    const auto x = a * test::e<1> + c * test::e<0>;
    const auto y = test::e<0>;

    const auto expected = -a * test::e<0, 1>;

    T::plus_count = 0;
    T::multiplies_count = 0;

    const auto r = x * y;

    return expect(
        same(expected, r) and     //
        eq(0, T::plus_count) and  //
        eq(1, T::multiplies_count));
  };

  "null generators pruned (2 blades)"_test = [] {
    struct tag
    {};
    using T = counted_double<tag>;
    using test = ::test<T>;

    const auto a = 3;
    const auto b = 4;
    const auto c = 2;

    const auto x = a * test::e<1> + b * test::e<2> + c * test::e<0>;
    const auto y = test::e<0>;

    const auto expected = -a * test::e<0, 1> - b * test::e<0, 2>;

    T::plus_count = 0;
    T::multiplies_count = 0;

    const auto r = x * y;

    return expect(
        same(expected, r) and     //
        eq(0, T::plus_count) and  //
        eq(2, T::multiplies_count));
  };

  "null generators pruned (3)"_test = [] {
    struct tag
    {};
    using T = counted_double<tag>;
    using test = ::test<T>;

    const auto a = 3;
    const auto b = 4;
    const auto c = 2;
    const auto d = 5;

    const auto x =
        a * test::e<1> + b * test::e<2> + c * test::e<0> + d * test::e<1, 2>;
    const auto y = test::e<0>;

    const auto expected =
        -a * test::e<0, 1> - b * test::e<0, 2> + d * test::e<0, 1, 2>;

    T::plus_count = 0;
    T::multiplies_count = 0;

    const auto r = x * y;

    return expect(
        same(expected, r) and     //
        eq(0, T::plus_count) and  //
        eq(3, T::multiplies_count));
  };

  "geometric product contracts to zero scalar"_ctest =  //
      [] {
        using test = ::test<double>;

        const auto a = 3;
        const auto b = 4;
        const auto c = 2;

        const auto x =
            a * test::e<0> + b * test::e<0, 1> + c * test::e<0, 1, 2>;

        return expect(same(test::blade<>{}, x * x));
      };
}
