#include <catch2/catch_test_macros.hpp>
#include "people/review.hpp"
TEST_CASE("Review") { Review r(5,"good"), bad(3,"ok"); REQUIRE(r.is_positive()); REQUIRE_FALSE(bad.is_positive()); REQUIRE_FALSE(r.is_published()); r.publish(); REQUIRE(r.is_published()); }
