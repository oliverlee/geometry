#include "geometry/geometry.hpp"
#include "skytest/skytest.hpp"

#include <cstddef>
#include <tuple>
#include <type_traits>

template <std::size_t... Is>
constexpr auto e = ::geometry::algebra<double, 2>::e<Is...>;

constexpr auto unit_blades = std::tuple{
    ::geometry::algebra<double, 2>::e<>,
    ::geometry::algebra<double, 2>::e<0>,
    ::geometry::algebra<double, 2>::e<1>,
    ::geometry::algebra<double, 2>::e<2>,
    ::geometry::algebra<double, 2>::e<0, 1>,
    ::geometry::algebra<double, 2>::e<0, 2>,
    ::geometry::algebra<double, 2>::e<1, 2>,
    ::geometry::algebra<double, 2>::e<0, 1, 2>};

auto main() -> int
{
  using namespace ::skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;
  using ::skytest::param_ref;

  "0-blade implicitly constructible"_ctest = [] {
    return expect(
        eq(e<>, e<>) and    //
        eq(0, 0 * e<>) and  //
        eq(1, 1 * e<>) and  //
        eq(-1, -e<>));
  };

  "grade 1+ blade explicitly constructible"_ctest * param_ref<unit_blades> =  //
      [](auto ei) {
        return expect(
            ei.grade == 0 or  //
            not std::is_convertible_v<double, decltype(ei)>);
      };

  "grade 0+ blades comparable"_ctest * param_ref<unit_blades> =  //
      [](auto ei) {
        using namespace ::skytest;

        return expect(
            eq(ei, ei) and      //
            ne(0 * ei, ei) and  //
            lt(0 * ei, ei) and  //
            le(0 * ei, ei) and  //
            gt(ei, 0 * ei) and  //
            ge(ei, 0 * ei));
      };
}
