#include <catch2/catch_test_macros.hpp>
#include "rooms/key_card.hpp"
TEST_CASE("Key card validity") { auto n=now_date(); KeyCard c(101,n+std::chrono::hours(24)); REQUIRE(c.is_active()); REQUIRE(c.is_valid(n)); c.deactivate(); REQUIRE_FALSE(c.is_active()); REQUIRE_FALSE(c.is_valid(n)); }
