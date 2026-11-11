#include <catch2/catch_test_macros.hpp>
#include "core/exception.hpp"
TEST_CASE("RoomNotAvailableException stores message") { RoomNotAvailableException e("x"); REQUIRE(e.getMessage() == "x"); }
