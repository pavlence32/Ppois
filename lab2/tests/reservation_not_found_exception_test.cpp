#include <catch2/catch_test_macros.hpp>
#include "core/exception.hpp"
TEST_CASE("ReservationNotFoundException stores message") { ReservationNotFoundException e("x"); REQUIRE(e.getMessage() == "x"); }
