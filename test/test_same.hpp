#pragma once

#include "skytest/skytest.hpp"

#include <ostream>
#include <string_view>
#include <type_traits>

namespace geometry::test {

template <class T>
struct typed
{
  T value;

  constexpr typed(const T& value) : value{value} {}

  friend auto& operator<<(std::ostream& os, typed x)
  {
    os << "<" << ::skytest::detail::type_name<T> << ">: " << x.value;
    return os;
  }

  template <class U>
  friend constexpr auto operator==(typed x, typed<U> y)
  {
    return std::is_same_v<T, U> and x.value == y.value;
  }
  template <class U>
  friend constexpr auto operator!=(typed x, typed<U> y)
  {
    return not(x == y);
  }
};

struct same_desc
{
  using notation_type = ::skytest::notation::function;
  static constexpr auto symbol = std::string_view{"same"};
};

inline constexpr auto same = [](const auto& x, const auto& y) {
  return ::skytest::pred(same_desc{}, [](const auto& x, const auto& y) {
    return ::skytest::eq(x, y);
  })(typed{x}, typed{y});
};

}  // namespace geometry::test
