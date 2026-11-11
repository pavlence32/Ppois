#include <catch2/catch_test_macros.hpp>
#include "core/exception.hpp"
TEST_CASE("DuplicateRoomException stores message") { DuplicateRoomException e("x"); REQUIRE(e.getMessage() == "x"); }
