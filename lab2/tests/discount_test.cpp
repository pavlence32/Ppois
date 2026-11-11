#include <catch2/catch_test_macros.hpp>
#include "finance/discount.hpp"
TEST_CASE("Discount") { auto n=now_date(); Discount d("sale",20,n+std::chrono::hours(24)); REQUIRE(d.apply(Money(100)).value()==80); REQUIRE(d.is_valid(n)); REQUIRE_FALSE(d.is_valid(n+std::chrono::hours(48))); REQUIRE_FALSE(d.is_full()); Discount full("full",100,n); REQUIRE(full.is_full()); }
