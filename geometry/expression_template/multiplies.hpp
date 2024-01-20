#pragma once

namespace geometry::expression_template {

struct multiplies
{
  static constexpr auto is_associative = true;

  template <class... Ts>
  constexpr auto operator()(const Ts&... ts) const -> decltype((ts * ...))
  {
    return (ts * ...);
  }
};

}  // namespace geometry::expression_template
