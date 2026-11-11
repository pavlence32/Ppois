#include <catch2/catch_test_macros.hpp>
#include "core/exception.hpp"
TEST_CASE("RoomNotCleanedException stores message") { RoomNotCleanedException e("x"); REQUIRE(e.getMessage() == "x"); }
