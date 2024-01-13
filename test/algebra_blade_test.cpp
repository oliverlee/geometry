#include "geometry/geometry.hpp"
#include "skytest/skytest.hpp"

#include <cstddef>
#include <sstream>
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

  "blade addition"_ctest * param_ref<unit_blades> =  //
      [](auto ei) { return expect(eq(3 * ei, ei + 2 * ei)); };

  "blade subtraction"_ctest * param_ref<unit_blades> =  //
      [](auto ei) { return expect(eq(ei, 3 * ei - 2 * ei)); };

  "blade addition assignment"_ctest * param_ref<unit_blades> =  //
      [](auto ei) {
        auto x = ei;
        return expect(eq(3 * ei, x += 2 * ei));
      };

  "blade subtraction assignment"_ctest * param_ref<unit_blades> =  //
      [](auto ei) {
        auto x = ei;
        return expect(eq(-ei, x -= 2 * ei));
      };

  "blade uses canonical dimensions"_ctest = [] {
    return expect(
        eq(-e<1, 0>, e<0, 1>) and        //
        eq(-e<2, 0>, e<0, 2>) and        //
        eq(-e<2, 1>, e<1, 2>) and        //
        eq(-e<0, 2, 1>, e<0, 1, 2>) and  //
        eq(e<2, 0, 1>, e<0, 1, 2>) and   //
        eq(-e<1, 0, 2>, e<0, 1, 2>) and  //
        eq(e<1, 2, 0>, e<0, 1, 2>) and   //
        eq(-e<2, 1, 0>, e<0, 1, 2>));
  };

  "duplicated (squared) basis vectors contract"_ctest = [] {
    return expect(
        eq(0, e<0, 0>) and  //
        eq(1, e<1, 1>) and  //
        eq(1, e<2, 2>));
  };

  "cayley table"_ctest = [] {
    return expect(
        eq(1, e<> * e<>) and                //
        eq(0, e<0> * e<0>) and              //
        eq(1, e<1> * e<1>) and              //
        eq(1, e<2> * e<2>) and              //
        eq(0, e<0, 1> * e<0, 1>) and        //
        eq(0, e<0, 2> * e<0, 2>) and        //
        eq(-1, e<1, 2> * e<1, 2>) and       //
        eq(0, e<0, 1, 2> * e<0, 1, 2>) and  //
        eq(e<0, 1>, e<0> * e<1>) and        //
        eq(e<0, 2>, e<0> * e<2>) and        //
        eq(e<1, 2>, e<1> * e<2>) and        //
        eq(-e<0, 1>, e<1> * e<0>) and       //
        eq(-e<0, 2>, e<2> * e<0>) and       //
        eq(-e<1, 2>, e<2> * e<1>) and       //
        eq(e<0, 2>, e<0, 1> * e<1, 2>) and  //
        eq(-e<0, 1>, e<0, 2> * e<1, 2>));
  };

  "geometric product"_ctest = [] {
    return expect(eq(6, (2 * e<1>)*(3 * e<1>)));
  };

  "printable"_test = [] {
    static const auto to_string = [](auto blade) {
      return (std::stringstream{} << blade).str();
    };

    return expect(
        eq("0*e0", to_string(0 * e<0>)) and          //
        eq("e1", to_string(1 * e<1>)) and            //
        eq("2*e2", to_string(2 * e<2>)) and          //
        eq("3*e01", to_string(3 * e<0, 1>)) and      //
        eq("4*e12", to_string(4 * e<1, 2>)) and      //
        eq("5*e02", to_string(5 * e<0, 2>)) and      //
        eq("6*e012", to_string(6 * e<0, 1, 2>)) and  //
        eq("7", to_string(7 * e<>)) and              //
        eq("1", to_string(e<>)));
  };

  "blade list R_2,0,1"_test = [] {
    using algebra = ::geometry::algebra<double, 2>;
    using ::geometry::type_list;

    return expect(
        std::is_same_v<
            type_list<
                algebra::blade<>,
                algebra::blade<0>,
                algebra::blade<1>,
                algebra::blade<2>,
                algebra::blade<0, 1>,
                algebra::blade<0, 2>,
                algebra::blade<1, 2>,
                algebra::blade<0, 1, 2>>,
            algebra::blade_list_type>);
  };

  "blade list R_3,0,1"_test = [] {
    using algebra = ::geometry::algebra<double, 3>;
    using ::geometry::type_list;

    return expect(
        std::is_same_v<
            type_list<
                algebra::blade<>,
                algebra::blade<0>,
                algebra::blade<1>,
                algebra::blade<2>,
                algebra::blade<3>,
                algebra::blade<0, 1>,
                algebra::blade<0, 2>,
                algebra::blade<0, 3>,
                algebra::blade<1, 2>,
                algebra::blade<1, 3>,
                algebra::blade<2, 3>,
                algebra::blade<0, 1, 2>,
                algebra::blade<0, 1, 3>,
                algebra::blade<0, 2, 3>,
                algebra::blade<1, 2, 3>,
                algebra::blade<0, 1, 2, 3>>,
            algebra::blade_list_type>);
  };
}
