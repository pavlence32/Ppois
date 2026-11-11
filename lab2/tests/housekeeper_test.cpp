#include <catch2/catch_test_macros.hpp>
#include "people/housekeeper.hpp"
TEST_CASE("Housekeeper status") { Housekeeper h(30,"Housekeeper","F",1,"clean"); REQUIRE_FALSE(h.get_status()); h.set_status(true); REQUIRE(h.get_status()); h.work(); }
