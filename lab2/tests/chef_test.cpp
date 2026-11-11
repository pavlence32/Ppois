#include <catch2/catch_test_macros.hpp>
#include "people/chef.hpp"
TEST_CASE("Chef status") { Chef c(30,"Chef","M",1,"cook"); REQUIRE_FALSE(c.get_status()); c.set_status(true); REQUIRE(c.get_status()); c.work(); }
