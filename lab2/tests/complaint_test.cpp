#include <catch2/catch_test_macros.hpp>
#include "people/complaint.hpp"
TEST_CASE("Complaint") { Complaint urgent("fire",8), normal("late",2); REQUIRE(urgent.is_urgent()); REQUIRE_FALSE(normal.is_urgent()); REQUIRE(urgent.is_open()); urgent.resolve(); REQUIRE_FALSE(urgent.is_open()); }
