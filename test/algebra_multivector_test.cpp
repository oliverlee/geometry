#include "geometry/geometry.hpp"
#include "skytest/skytest.hpp"

#include <cstddef>
#include <sstream>
#include <tuple>
#include <type_traits>

template <std::size_t... Is>
constexpr auto e = ::geometry::algebra<double, 2>::e<Is...>;

/// construct a multivector from blades
///
inline constexpr struct
{
  template <
      class... Bs,
      class = std::enable_if_t<
          sizeof...(Bs) != 0 and
          ::geometry::tmp::same_v<typename Bs::algebra_type...>>>
  [[nodiscard]]
  constexpr auto
  operator()(Bs... bs) const
  {
    using Algebra = std::common_type_t<typename Bs::algebra_type...>;
    return typename Algebra::template multivector<Bs...>{bs...};
  }
} multivector{};

auto main() -> int
{
  using namespace ::skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;
  using ::skytest::param_ref;

  static constexpr auto multivectors = std::tuple{
      multivector(e<>),
      multivector(e<>, e<0>),
      multivector(e<>, e<1>),
      multivector(e<>, e<2>),
      multivector(e<>, e<0>, e<1, 2>),
      multivector(e<>, e<0, 1>, e<1, 2>),
      multivector(e<>, e<0, 1, 2>)};

  "multivector with same elements comparable"_ctest *
      param_ref<multivectors> =  //
      [](auto v) { return expect(eq(v, v)); };

  "multivector with different elements comparable"_ctest =  //
      [] {
        return expect(
            eq(0 * e<0> + e<1>, e<1> + 0 * e<2>) and  //
            eq(e<1>, e<1> + 0 * e<2>) and             //
            eq(0 * e<0> + e<1>, e<1>));
      };

  "multivector constructible from blade addition/subtraction"_ctest =  //
      [] {
        return expect(
            eq(multivector(2 * e<0>, 3 * e<1>), 2 * e<0> + 3 * e<1>) and
            eq(multivector(2 * e<0>, 3 * e<1>), 3 * e<1> + 2 * e<0>) and
            eq(multivector(-2 * e<0>, 3 * e<1>), 3 * e<1> - 2 * e<0>));
      };

  "multivector negation"_ctest =  //
      [] {
        return expect(
            eq(e<0> + e<1>, -(-e<0> - e<1>)) and
            eq(e<0, 1> + e<1, 2>, -(-e<0, 1> - e<1, 2>)));
      };

  "multivectors addable"_ctest =  //
      [] {
        constexpr auto z = multivector(2 * e<0>, 3 * e<1>, 4 * e<2>);

        return expect(
            eq(z, 2 * e<0> + 3 * e<1> + 4 * e<2>) and    //
            eq(z, (2 * e<0> + 3 * e<1>)+4 * e<2>) and    //
            eq(z, 2 * e<0> + (3 * e<1> + 4 * e<2>)) and  //
            eq(z, z + 0 * e<0>) and                      //
            eq(z, z + 0 * e<0> + 0 * e<1>) and           //
            eq(z, z + (0 * e<0> + 0 * e<1>)) and

            eq(2 * e<0> + 3 * e<1> + 4 * e<2>, z) and    //
            eq((2 * e<0> + 3 * e<1>)+4 * e<2>, z) and    //
            eq(2 * e<0> + (3 * e<1> + 4 * e<2>), z) and  //
            eq(z + 0 * e<0>, z) and                      //
            eq(z + 0 * e<0> + 0 * e<1>, z) and           //
            eq(z + (0 * e<0> + 0 * e<1>), z));
      };

  "multivectors subtractible"_ctest =  //
      [] {
        constexpr auto z = multivector(2 * e<0>, 3 * e<1>, 4 * e<2>);

        return expect(
            eq(z, z + z - z) and  //
            eq(-z, decltype(z){} - z) and
            eq(z, (2 * e<0> + 4 * e<1>)-(e<1> - 4 * e<2>)) and  //
            eq(z, z - 0 * e<0>) and eq(-z, 0 * e<0> - z));
      };

  "squared 1-vector"_ctest =  //
      [] {
        const auto a = 3;
        const auto b = 4;
        const auto c = 2;

        const auto m = a * e<1> + b * e<2> + c * e<0>;

        return expect(eq(a * a + b * b, m * m));
      };

  "squared 2-vector"_ctest =  //
      [] {
        const auto x = 3;
        const auto y = 4;
        const auto z = 2;

        const auto P = x * e<0, 2> + y * e<0, 1> + z * e<1, 2>;

        return expect(eq(-z * z, P * P));
      };

  "geometric product of multivector with blade"_ctest =  //
      [] {
        const auto a = 3;
        const auto b = 4;
        const auto c = 2;

        const auto x = a * e<1> + b * e<2> + c * e<0>;
        const auto y = e<0>;

        const auto z = -a * e<0, 1> - b * e<0, 2>;

        return expect(eq(z, x * y) and eq(-z, y * x));
      };

  "geometric product sfinae friendly"_ctest = [] {
    using a1 = ::geometry::algebra<double, 3>;
    using a2 = ::geometry::algebra<float, 3>;

    using F = decltype(::geometry::geometric_product);

    return expect(
        not std::is_invocable_v<F, int, int> and
        not std::is_invocable_v<F, a1::blade<>, a2::blade<>>);
  };

  "printable"_test = [] {
    static const auto to_string = [](const auto& mvec) {
      return (std::stringstream{} << mvec).str();
    };

    return expect(
        eq("1", to_string(multivector(e<>))) and
        eq("2*e0 + 3*e1 + 4*e2", to_string(2 * e<0> + 3 * e<1> + 4 * e<2>))

    );
  };
}
