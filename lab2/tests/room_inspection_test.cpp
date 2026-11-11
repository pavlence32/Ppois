#include <catch2/catch_test_macros.hpp>
#include "rooms/room_inspection.hpp"
TEST_CASE("Room inspection") { RoomInspection i(101,"Inspector"); REQUIRE_FALSE(i.passed()); REQUIRE(i.concerns_room(101)); REQUIRE_FALSE(i.concerns_room(102)); i.pass(); REQUIRE(i.passed()); }
