#include "geometry/geometry.hpp"
#include "skytest/skytest.hpp"

#include <cstddef>
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
          ::geometry::detail::all_same_v<typename Bs::algebra_type...>>>
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

  "multivector constructible from blade addition"_ctest =  //
      [] {
        return expect(
            eq(multivector(2 * e<0>, 3 * e<1>), 2 * e<0> + 3 * e<1>) and
            eq(multivector(2 * e<0>, 3 * e<1>), 3 * e<1> + 2 * e<0>) and
            eq(multivector(-2 * e<0>, 3 * e<1>), 3 * e<1> - 2 * e<0>));
      };
}
