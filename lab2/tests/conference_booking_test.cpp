#include <catch2/catch_test_macros.hpp>
#include "hotel/conference_booking.hpp"
TEST_CASE("Conference booking") { ConferenceBooking b(10,Money(100)); REQUIRE(b.total().value()==1000); REQUIRE_FALSE(b.is_reserved()); b.reserve(); REQUIRE(b.is_reserved()); }
