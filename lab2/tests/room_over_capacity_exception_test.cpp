#include <catch2/catch_test_macros.hpp>
#include "core/exception.hpp"
TEST_CASE("RoomOverCapacityException stores message") { RoomOverCapacityException e("x"); REQUIRE(e.getMessage() == "x"); }
