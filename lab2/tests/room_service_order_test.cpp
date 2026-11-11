#include <catch2/catch_test_macros.hpp>
#include "services/room_service_order.hpp"
TEST_CASE("Room service order") { RoomServiceOrder o(Money(25)); REQUIRE(o.price().value()==25); REQUIRE_FALSE(o.is_delivered()); o.deliver(); REQUIRE(o.is_delivered()); }
