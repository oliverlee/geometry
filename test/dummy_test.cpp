#include "geometry/geometry.hpp"
#include "skytest/skytest.hpp"

auto main() -> int
{
  using namespace ::skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;

  "dummy"_ctest = [] { return expect(eq(0, ::geometry::dummy_function())); };
}
