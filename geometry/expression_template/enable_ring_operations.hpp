#pragma once

#include <functional>
#include <type_traits>

namespace geometry::expression_template {

template <class D>
class enable_ring_operations
{
  friend D;

  template <class D2>
  friend class enable_ring_operations;

  using derived_type = D;

  enable_ring_operations() = default;
  enable_ring_operations(const enable_ring_operations&) = default;

  constexpr auto derived() const
  {
    return static_cast<const derived_type&>(*this);
  }

  template <class D2, class D1 = D>
  friend constexpr auto operator+(
      const enable_ring_operations& x, const enable_ring_operations<D2>& y)
      -> std::enable_if_t<
          std::is_invocable_v<
              std::plus<>,
              typename D1::eval_type,
              typename D2::eval_type>,
          typename D2::template expression<std::plus<>, D1, D2>>
  {
    return {x.derived(), y.derived()};
  }

  template <class D2, class D1 = D>
  friend constexpr auto operator*(
      const enable_ring_operations& x, const enable_ring_operations<D2>& y)
      -> std::enable_if_t<
          std::is_invocable_v<
              std::multiplies<>,
              typename D1::eval_type,
              typename D2::eval_type>,
          typename D2::template expression<std::multiplies<>, D1, D2>>
  {
    return {x.derived(), y.derived()};
  }
};

}  // namespace geometry::expression_template
