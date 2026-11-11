#include <catch2/catch_test_macros.hpp>
#include "core/exception.hpp"
TEST_CASE("KeyExpiredException stores message") { KeyExpiredException e("x"); REQUIRE(e.getMessage() == "x"); }
