#include <catch2/catch_test_macros.hpp>
#include "services/mini_bar.hpp"
TEST_CASE("Mini bar") { MiniBar m(2,Money(20)); REQUIRE(m.price().value()==20); REQUIRE_FALSE(m.is_sealed()); m.seal(); REQUIRE(m.is_sealed()); }
