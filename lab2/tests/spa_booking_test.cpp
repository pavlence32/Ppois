#include <catch2/catch_test_macros.hpp>
#include "services/spa_booking.hpp"
TEST_CASE("Spa booking") { SpaBooking b("massage",Money(40)); REQUIRE(b.price().value()==40); REQUIRE_FALSE(b.is_completed()); b.complete(); REQUIRE(b.is_completed()); }
