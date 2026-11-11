#include <catch2/catch_test_macros.hpp>
#include "people/receptionist.hpp"
TEST_CASE("Receptionist status") { receptionist r(30,"Reception","F",1,"front"); REQUIRE(r.get_status()); r.set_status(false); REQUIRE_FALSE(r.get_status()); r.work(); }
