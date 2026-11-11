#include <catch2/catch_test_macros.hpp>
#include "hotel/parking_ticket.hpp"
TEST_CASE("Parking ticket") { ParkingTicket t("1","ABC"); REQUIRE(t.is_active()); REQUIRE(t.belongs_to_car("ABC")); REQUIRE_FALSE(t.belongs_to_car("XYZ")); t.close(); REQUIRE_FALSE(t.is_active()); }
