#include <catch2/catch_test_macros.hpp>
#include "core/model.hpp"
TEST_CASE("Money arithmetic and predicates") { Money a(100), b(25); REQUIRE(a.value()==100); REQUIRE((a * 2.5).value()==250); REQUIRE((a+b).value()==125); REQUIRE((a-b).value()==75); REQUIRE(a.percent(10).value()==10); REQUIRE(a.add_tax(20).value()==120); REQUIRE(a.apply_discount(15).value()==85); REQUIRE_FALSE(a.is_zero()); REQUIRE(a.is_positive()); REQUIRE(Money().is_zero()); REQUIRE_FALSE(Money(-1).is_positive()); }
TEST_CASE("Date helpers") { auto n=now_date(); auto later=n+std::chrono::hours(48); REQUIRE(valid_date_range(n,later)); REQUIRE_FALSE(valid_date_range(later,n)); REQUIRE(days_between(n,later)==2); REQUIRE(date_has_passed(n-std::chrono::hours(1),n)); REQUIRE_FALSE(date_has_passed(later,n)); }
