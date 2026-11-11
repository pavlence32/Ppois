#include <catch2/catch_test_macros.hpp>
#include "hotel/hotel_event.hpp"
TEST_CASE("Hotel event") { auto n=now_date(); HotelEvent e("concert",n,10), empty("",n,0); REQUIRE(e.has_name()); REQUIRE_FALSE(empty.has_name()); REQUIRE(e.has_started(n)); REQUIRE(e.can_accept(10)); REQUIRE_FALSE(e.can_accept(11)); }
