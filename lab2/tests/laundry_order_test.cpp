#include <catch2/catch_test_macros.hpp>
#include "services/laundry_order.hpp"
TEST_CASE("Laundry order") { LaundryOrder o(3,Money(4)); REQUIRE(o.total().value()==12); REQUIRE_FALSE(o.is_ready()); o.mark_ready(); REQUIRE(o.is_ready()); }
