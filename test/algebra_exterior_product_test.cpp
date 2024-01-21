#include "geometry/geometry.hpp"
#include "skytest/skytest.hpp"

#include "test/test_same.hpp"

auto main() -> int
{
  using namespace ::skytest::literals;
  using ::geometry::test::same;
  using ::skytest::eq;
  using ::skytest::expect;

  constexpr auto algebras = ::skytest::types<
      ::geometry::algebra<float, 2>,
      ::geometry::algebra<float, 3>,
      ::geometry::algebra<double, 2>,
      ::geometry::algebra<double, 3>>;

  "unique basis vectors"_ctest * algebras = [](auto param) {
    using algebra = typename decltype(param)::type;

    return expect(
        same(algebra::template e<0, 1>,
             algebra::template e<0> ^ algebra::template e<1>) and
        same(algebra::template e<1, 2>,
             algebra::template e<1> ^ algebra::template e<2>));
  };

  "psuedoscalar ^ psuedoscalar"_ctest * algebras = [](auto param) {
    using algebra = typename decltype(param)::type;

    return expect(same(0 * algebra::i, algebra::i ^ algebra::i));
  };

  "ei ^ ei returns zero pseudoscalar"_ctest * algebras = [](auto param) {
    using algebra = typename decltype(param)::type;

    constexpr auto zero = 0 * algebra::i;

    return expect(
        same(zero, algebra::template e<0> ^ algebra::template e<0>) and
        same(zero, algebra::template e<1> ^ algebra::template e<1>) and
        same(zero, algebra::template e<2> ^ algebra::template e<2>));
  };

  "v ^ v for 1-vector"_ctest * algebras = [](auto param) {
    using algebra = typename decltype(param)::type;

    const auto a = 1;
    const auto b = 2;
    const auto c = 3;

    const auto v =
        a * algebra::template e<0> + b * algebra::template e<1> +
        c * algebra::template e<2>;

    return expect(eq(0, v ^ v));
  };

  "u ^ v for 1-vectors"_ctest * algebras = [](auto param) {
    using algebra = typename decltype(param)::type;

    const auto a = 1;
    const auto b = 2;
    const auto c = 3;

    const auto u =
        algebra::template e<0> + algebra::template e<1> +
        algebra::template e<2>;
    const auto v =
        a * algebra::template e<0> + b * algebra::template e<1> +
        c * algebra::template e<2>;

    const auto r =
        (b - a) * algebra::template e<0, 1> +
        (c - b) * algebra::template e<1, 2> +
        (a - c) * algebra::template e<2, 0>;

    return expect(same(r, u ^ v));
  };

  "A ^ B for 2-vectors (pga2)"_ctest = [] {
    using algebra = ::geometry::algebra<double, 2>;

    const auto a = 1;
    const auto b = 2;
    const auto c = 3;

    const auto A =
        algebra::template e<0, 1> + algebra::template e<1, 2> +
        algebra::template e<2, 0>;
    const auto B =
        a * algebra::template e<0, 1> + b * algebra::template e<1, 2> +
        c * algebra::template e<2, 0>;

    constexpr auto zero = 0 * algebra::i;

    return expect(same(zero, A ^ B));
  };

  "A ^ B for 2-vectors (pga3)"_ctest = [] {
    using algebra = ::geometry::algebra<double, 3>;

    const auto a = 2;
    const auto b = 3;

    const auto A = algebra::template e<0, 1> + algebra::template e<2, 3>;
    const auto B =
        a * algebra::template e<0, 1> + b * algebra::template e<2, 3>;

    constexpr auto R = (b + a) * algebra::i;

    return expect(same(R, A ^ B));
  };
}
