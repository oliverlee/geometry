#include "geometry/geometry.hpp"
#include "skytest/skytest.hpp"

#include "test/test_same.hpp"

#include <cstddef>
#include <tuple>
#include <utility>

constexpr class
{
  template <std::size_t... Is, class T, class P>
  static constexpr auto impl(std::index_sequence<Is...>, const T& args, P pred)
  {
    return (pred(std::get<Is>(args)) and ...);
  }

public:
  template <class... Ts, class P>
  constexpr auto operator()(const std::tuple<Ts...>& args, P pred) const
  {
    return impl(std::index_sequence_for<Ts...>{}, args, pred);
  }
} for_each{};

auto main() -> int
{
  using namespace ::skytest::literals;
  using ::geometry::test::same;
  using ::skytest::eq;
  using ::skytest::expect;

  "dual (hodge star)"_ctest = [] {
    using algebra = ::geometry::algebra<double, 3>;
    using dual_algebra = typename algebra::dual_algebra_type;
    using ::geometry::dual;

    // https://bivector.net/PGA4CS.pdf#table.4
    // clang-format off
    return expect(
        same(dual(algebra::template e<>), dual_algebra::template e<0, 1, 2, 3>) and
        same(dual(algebra::template e<0>), dual_algebra::template e<1, 2, 3>) and
        same(dual(algebra::template e<1>), dual_algebra::template e<0, 3, 2>) and
        same(dual(algebra::template e<2>), dual_algebra::template e<0, 1, 3>) and
        same(dual(algebra::template e<3>), dual_algebra::template e<0, 2, 1>) and
        same(dual(algebra::template e<0, 1>), dual_algebra::template e<2, 3>) and
        same(dual(algebra::template e<0, 2>), dual_algebra::template e<3, 1>) and
        same(dual(algebra::template e<0, 3>), dual_algebra::template e<1, 2>) and
        same(dual(algebra::template e<2, 3>), dual_algebra::template e<0, 1>) and
        same(dual(algebra::template e<3, 1>), dual_algebra::template e<0, 2>) and
        same(dual(algebra::template e<1, 2>), dual_algebra::template e<0, 3>) and
        same(dual(algebra::template e<1, 2, 3>), -dual_algebra::template e<0>) and
        same(dual(algebra::template e<0, 3, 2>), -dual_algebra::template e<1>) and
        same(dual(algebra::template e<0, 1, 3>), -dual_algebra::template e<2>) and
        same(dual(algebra::template e<0, 2, 1>), -dual_algebra::template e<3>) and
        same(dual(algebra::template e<0, 1, 2, 3>), dual_algebra::template e<>));
    // clang-format on
  };

  constexpr auto algebras = ::skytest::
      types<::geometry::algebra<float, 2>, ::geometry::algebra<double, 3>>;

  "dual and undual"_ctest * algebras = [](auto param) {
    using algebra = typename decltype(param)::type;

    constexpr auto test_blade = std::tuple{
        algebra::template e<>,
        algebra::template e<0>,
        algebra::template e<1>,
        algebra::template e<2>,
        algebra::template e<0, 1>,
        algebra::template e<1, 2>,
        algebra::template e<2, 0>,
        algebra::template e<0, 1, 2>,
        algebra::i};

    constexpr auto compare_with_dual_then_undual = [](const auto& arg) {
      using ::geometry::dual;
      using ::geometry::undual;

      return same(arg, undual(dual(arg)));
    };

    return expect(for_each(test_blade, compare_with_dual_then_undual));
  };

  "join of two points"_ctest = [] {
    using algebra = ::geometry::algebra<double, 2>;

    constexpr auto x_hat = algebra::e<2, 0>;
    constexpr auto y_hat = algebra::e<0, 1>;
    constexpr auto E0 = algebra::e<1, 2>;

    const auto P = 0 * x_hat + 3 * y_hat + E0;
    const auto Q = 2 * x_hat + 3 * y_hat + E0;

    // regressive product (join)
    using ::geometry::dual;
    using ::geometry::undual;
    const auto l1 = undual(dual(P) ^ dual(Q));
    const auto l2 = undual(dual(Q) ^ dual(P));

    using e0 = algebra::blade<0>;
    using x = algebra::blade<1>;
    using y = algebra::blade<2>;

    // a line of the form
    // a * x + b * y + c = 0
    // is represented as
    // a * e1 + b * e2 + c * e0 = 0

    using geometry::get;
    return expect(
        eq(0, get<x>(l1).coefficient) and
        eq(-3, get<e0>(l1).coefficient / get<y>(l1).coefficient) and
        eq(l1, -l2));
  };
}
