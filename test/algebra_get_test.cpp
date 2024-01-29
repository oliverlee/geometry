#include "geometry/geometry.hpp"
#include "skytest/skytest.hpp"

#include <utility>

template <std::size_t... Is>
constexpr auto e = ::geometry::algebra<double, 2>::e<Is...>;

template <std::size_t... Is>
using blade = typename ::geometry::algebra<double, 2>::template blade<Is...>;

auto main() -> int
{
  using namespace ::skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;

  "get lvalue ref"_ctest = [] {
    auto x = e<> + e<0>;

    using ::geometry::get;

    static_assert(std::is_same_v<blade<>&, decltype(get<blade<>>(x))>);
    static_assert(
        std::is_same_v<blade<>&, decltype(::geometry::get<blade<>>(x))>);

    static_assert(std::is_same_v<blade<0>&, decltype(get<blade<0>>(x))>);
    static_assert(
        std::is_same_v<blade<0>&, decltype(::geometry::get<blade<0>>(x))>);

    return expect(true);
  };

  "get const lvalue ref"_ctest = [] {
    const auto x = e<> + e<0>;

    using ::geometry::get;

    static_assert(std::is_same_v<const blade<>&, decltype(get<blade<>>(x))>);
    static_assert(
        std::is_same_v<const blade<>&, decltype(::geometry::get<blade<>>(x))>);

    static_assert(std::is_same_v<const blade<0>&, decltype(get<blade<0>>(x))>);
    static_assert(
        std::
            is_same_v<const blade<0>&, decltype(::geometry::get<blade<0>>(x))>);

    return expect(true);
  };

  "get rvalue ref"_ctest = [] {
    auto x = e<> + e<0>;

    using ::geometry::get;

    static_assert(
        std::is_same_v<blade<>&&, decltype(get<blade<>>(std::move(x)))>);
    static_assert(
        std::is_same_v<
            blade<>&&,
            decltype(::geometry::get<blade<>>(std::move(x)))>);

    static_assert(
        std::is_same_v<blade<0>&&, decltype(get<blade<0>>(std::move(x)))>);
    static_assert(
        std::is_same_v<
            blade<0>&&,
            decltype(::geometry::get<blade<0>>(std::move(x)))>);

    return expect(true);
  };

  "get const rvalue ref"_ctest = [] {
    const auto x = e<> + e<0>;

    using ::geometry::get;

    // NOLINTBEGIN(performance-move-const-arg)

    static_assert(
        std::is_same_v<const blade<>&&, decltype(get<blade<>>(std::move(x)))>);
    static_assert(
        std::is_same_v<
            const blade<>&&,
            decltype(::geometry::get<blade<>>(std::move(x)))>);

    static_assert(
        std::
            is_same_v<const blade<0>&&, decltype(get<blade<0>>(std::move(x)))>);
    static_assert(
        std::is_same_v<
            const blade<0>&&,
            decltype(::geometry::get<blade<0>>(std::move(x)))>);

    // NOLINTEND(performance-move-const-arg)

    return expect(true);
  };

  "get not used with multivector"_ctest = [] {
    const auto b = e<1>;

    using geometry::get;
    auto f = [](auto x) -> decltype(get<blade<1>>(x)) {};

    static_assert(not std::is_invocable_v<decltype(f), decltype(b)>);
    static_assert(
        not std::
            is_invocable_v<decltype(::geometry::get<blade<1>>), decltype(b)>);

    return expect(true);
  };

  "blade not in multivector"_ctest = [] {
    const auto x = e<> + e<0>;

    using ::geometry::get;
    auto f = [](auto x) -> decltype(get<blade<1>>(x)) {};

    static_assert(not std::is_invocable_v<decltype(f), decltype(x)>);
    static_assert(
        not std::
            is_invocable_v<decltype(::geometry::get<blade<1>>), decltype(x)>);

    return expect(true);
  };
}
