#include <catch2/catch_test_macros.hpp>
#include "services/transport_request.hpp"
TEST_CASE("Transport request") { TransportRequest r(2,Money(30)); REQUIRE(r.price().value()==30); REQUIRE_FALSE(r.is_confirmed()); r.confirm(); REQUIRE(r.is_confirmed()); }
