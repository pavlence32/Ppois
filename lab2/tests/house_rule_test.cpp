#include <catch2/catch_test_macros.hpp>
#include "hotel/house_rule.hpp"
TEST_CASE("House rule") { HouseRule r("No smoking","description"); REQUIRE(r.is_active()); REQUIRE(r.applies_to("No smoking")); REQUIRE_FALSE(r.applies_to("Pets")); r.disable(); REQUIRE_FALSE(r.is_active()); }
