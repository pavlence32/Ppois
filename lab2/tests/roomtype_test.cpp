#include <catch2/catch_test_macros.hpp>
#include "rooms/Roomtype.hpp"
TEST_CASE("Room type") { RoomType t("suite",2.0f); REQUIRE(t.get_type()=="suite"); REQUIRE(t.get_priceMultiplier()==2.0f); }
