#include <catch2/catch_test_macros.hpp>
#include "core/exception.hpp"
TEST_CASE("RoomNotFoundException stores message") { RoomNotFoundException e("x"); REQUIRE(e.getMessage() == "x"); }
