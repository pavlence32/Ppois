#include <catch2/catch_test_macros.hpp>
#include "services/service.hpp"
TEST_CASE("Service prices") { Service s("spa",Money(50)), free("free"); REQUIRE(s.get_name()=="spa"); REQUIRE(s.get_price().value()==50); REQUIRE(s.price_for(2).value()==100); REQUIRE(s.price_with_discount(2,10).value()==90); REQUIRE_FALSE(s.is_free()); REQUIRE(free.is_free()); }
