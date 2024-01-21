#include "geometry/geometry.hpp"
#include "skytest/skytest.hpp"

#include <cstddef>
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

auto main() -> int
{
  using namespace ::skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;

  "null generators pruned (0 blades)"_ctest = [] {
    using test = ::test<double>;

    const auto c = 2;

    const auto x = c * test::e<0> + test::e<0, 1>;
    const auto y = test::e<0>;

    auto r = x * y;

    return expect(std::is_same_v<test::blade<>, decltype(r)>);
  };

  "null generators pruned (1 blade)"_ctest = [] {
    using test = ::test<double>;

    const auto a = 3;
    const auto c = 2;

    const auto x = a * test::e<1> + c * test::e<0>;
    const auto y = test::e<0>;

    auto r = x * y;

    return expect(std::is_same_v<test::blade<0, 1>, decltype(r)>);
  };

  "null generators pruned (2 blades)"_test = [] {
    using test = ::test<double>;

    const auto a = 3;
    const auto b = 4;
    const auto c = 2;

    const auto x = a * test::e<1> + b * test::e<2> + c * test::e<0>;
    const auto y = test::e<0>;

    auto r = x * y;

    return expect(
        std::is_same_v<
            test::multivector<test::blade<0, 1>, test::blade<0, 2>>,
            decltype(r)>);
  };

  "null generators pruned (3 blades)"_test = [] {
    using test = ::test<double>;

    const auto a = 3;
    const auto b = 4;
    const auto c = 2;
    const auto d = 5;

    const auto x =
        a * test::e<1> + b * test::e<2> + c * test::e<0> + d * test::e<1, 2>;
    const auto y = test::e<0>;

    auto r = x * y;

    return expect(
        std::is_same_v<
            test::multivector<
                test::blade<0, 1>,
                test::blade<0, 2>,
                test::blade<0, 1, 2>>,
            decltype(r)>);
  };
}
