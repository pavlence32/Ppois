#include <catch2/catch_test_macros.hpp>
#include "services/pool_visit.hpp"
TEST_CASE("Pool visit") { auto n=now_date(); PoolVisit p(2,n); REQUIRE(p.is_today(n)); REQUIRE_FALSE(p.is_checked()); p.check(); REQUIRE(p.is_checked()); }
