#include <catch2/catch_test_macros.hpp>
#include "services/breakfast_order.hpp"
TEST_CASE("Breakfast order") { BreakfastOrder o(2,Money(10)); REQUIRE(o.total().value()==20); REQUIRE_FALSE(o.is_delivered()); o.deliver(); REQUIRE(o.is_delivered()); }
