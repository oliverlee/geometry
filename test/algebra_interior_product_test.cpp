#include "geometry/geometry.hpp"
#include "skytest/skytest.hpp"

#include "test/test_same.hpp"

#include <cstddef>
#include <tuple>
#include <utility>

auto main() -> int
{
  using namespace ::skytest::literals;
  using ::geometry::get;
  using ::geometry::test::same;
  using ::skytest::eq;
  using ::skytest::expect;
  using ::skytest::ne;

  "line orthogonal to point (2D)"_ctest = [] {
    using algebra = ::geometry::algebra<double, 2>;

    // https://bivector.net/PROJECTIVE_GEOMETRIC_ALGEBRA.pdf
    // section 7.1

    constexpr auto x_hat = algebra::e<2, 0>;
    constexpr auto y_hat = algebra::e<0, 1>;
    constexpr auto E0 = algebra::e<1, 2>;

    const auto P = 0 * x_hat + 0 * y_hat + E0;

    // a line of the form
    // a * x + b * y + c = 0
    // is represented as
    // a * e1 + b * e2 + c * e0 = 0

    //using e0 = algebra::blade<0>;
    constexpr auto x = algebra::e<1>;
    constexpr auto y = algebra::e<2>;

    return expect(eq(x, y | P) and
                  eq(y, x | P));
  };

}
