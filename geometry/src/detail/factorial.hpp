#pragma once

#include <cstddef>

namespace geometry::detail {

constexpr auto factorial(std::size_t n)
{
  auto a = std::size_t{1};

  if (n == 0) {
    return a;
  }

  while (n != 1) {
    a *= n--;
  }

  return a;
}

}  // namespace geometry::detail
