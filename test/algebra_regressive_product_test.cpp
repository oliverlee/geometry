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

  "join of two points (2D)"_ctest = [] {
    using algebra = ::geometry::algebra<double, 2>;

    // https://bivector.net/PROJECTIVE_GEOMETRIC_ALGEBRA.pdf
    // section 7.1

    constexpr auto x_hat = algebra::e<2, 0>;
    constexpr auto y_hat = algebra::e<0, 1>;
    constexpr auto E0 = algebra::e<1, 2>;

    const auto P = 0 * x_hat + 3 * y_hat + E0;
    const auto Q = 2 * x_hat + 3 * y_hat + E0;

    const auto l1 = P & Q;
    const auto l2 = Q & P;

    using e0 = algebra::blade<0>;
    using x = algebra::blade<1>;
    using y = algebra::blade<2>;

    // a line of the form
    // a * x + b * y + c = 0
    // is represented as
    // a * e1 + b * e2 + c * e0 = 0

    return expect(
        eq(0, get<x>(l1).coefficient) and
        eq(-3, get<e0>(l1).coefficient / get<y>(l1).coefficient) and
        eq(l1, -l2));
  };

  "join of two points (3D)"_ctest = [] {
    using algebra = ::geometry::algebra<double, 3>;

    constexpr auto x_hat = algebra::e<0, 3, 2>;
    constexpr auto y_hat = algebra::e<0, 1, 3>;
    constexpr auto z_hat = algebra::e<0, 2, 1>;
    constexpr auto E0 = algebra::e<1, 2, 3>;

    const auto P = 0 * x_hat + 3 * y_hat + 1 * z_hat + E0;
    const auto Q = 1 * x_hat + 3 * y_hat + 1 * z_hat + E0;

    // https://en.wikipedia.org/wiki/Pl%C3%BCcker_coordinates

    const auto l1 = P & Q;  // (m; d) where m = e01, e02, e03 components
    const auto l2 = Q & P;  //              d = e23, e31, e12 components

    // https://bivector.net/PROJECTIVE_GEOMETRIC_ALGEBRA.pdf
    // section 8, notations and foundations

    using ix = algebra::blade<0, 1>;
    using iy = algebra::blade<0, 2>;
    using iz = algebra::blade<0, 3>;

    const auto m =
        get<ix>(l1).coefficient * x_hat +  //
        get<iy>(l1).coefficient * y_hat +  //
        get<iz>(l1).coefficient * z_hat;

    const auto d = m & (P - E0);

    using x = algebra::blade<2, 3>;
    using y = algebra::blade<1, 3>;  // sign would be flipped if non-zero
    using z = algebra::blade<1, 2>;

    return expect(
        ne(0, get<x>(l1).coefficient) and                //
        eq(0, get<y>(l1).coefficient) and                //
        eq(0, get<z>(l1).coefficient) and                //
        eq(ix{1}, d * (1 / get<ix>(d).coefficient)) and  //
        eq(l1, -l2));
  };

  "join of three points (3D)"_ctest = [] {
    using algebra = ::geometry::algebra<double, 3>;

    constexpr auto x_hat = algebra::e<0, 3, 2>;
    constexpr auto y_hat = algebra::e<0, 1, 3>;
    constexpr auto z_hat = algebra::e<0, 2, 1>;
    constexpr auto E0 = algebra::e<1, 2, 3>;

    const auto P = 1 * x_hat + 0 * y_hat + 0 * z_hat + E0;
    const auto Q = 0 * x_hat + 2 * y_hat + 0 * z_hat + E0;
    const auto R = 0 * x_hat + 0 * y_hat + 3 * z_hat + E0;

    // plane p = a*e1 + b*e2 + c*e3 + d*e0
    //       0 = a*x  + b*y  + c*z  + d
    const auto p = P & Q & R;

    using e0 = algebra::blade<0>;
    using x = algebra::blade<1>;
    using y = algebra::blade<2>;
    using z = algebra::blade<3>;

    const auto d = get<e0>(p).coefficient;

    return expect(
        eq(-d, get<x>(p).coefficient) and      //
        eq(-d / 2, get<y>(p).coefficient) and  //
        eq(-d / 3, get<z>(p).coefficient));
  };
}
