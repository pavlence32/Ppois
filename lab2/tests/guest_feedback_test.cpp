#include <catch2/catch_test_macros.hpp>
#include "people/guest_feedback.hpp"
TEST_CASE("Guest feedback") { GuestFeedback good("great",4), bad("bad",3); REQUIRE(good.is_positive()); REQUIRE_FALSE(bad.is_positive()); REQUIRE_FALSE(good.is_processed()); good.process(); REQUIRE(good.is_processed()); }
