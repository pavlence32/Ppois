#include <catch2/catch_test_macros.hpp>
#include "rooms/amenity.hpp"
TEST_CASE("Amenity lifecycle and price") { Amenity a("wifi",AmenityCategory::Technology,true,Money(5)); REQUIRE(a.get_name()=="wifi"); REQUIRE(a.is_paid()); REQUIRE(a.get_cost(3).value()==15); REQUIRE(a.is_available_for(3)); a.mark_broken(); REQUIRE_FALSE(a.is_usable()); REQUIRE_FALSE(a.is_available_for(3)); a.repair(); a.change_price(Money(7)); REQUIRE(a.get_cost(2).value()==14); }
