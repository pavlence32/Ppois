#include <catch2/catch_test_macros.hpp>
#include "people/guest_badge.hpp"
TEST_CASE("Guest badge lifecycle") { GuestBadge b("Ann"); REQUIRE(b.is_active()); REQUIRE(b.belongs_to("Ann")); REQUIRE_FALSE(b.belongs_to("Bob")); b.deactivate(); REQUIRE_FALSE(b.is_active()); }
