#include <catch2/catch_test_macros.hpp>
#include "rooms/suite.hpp"
#include "core/exception.hpp"

TEST_CASE("Suite amenities and pricing")
{
    Service included("breakfast", Money(20));
    Suite suite(301, nullptr, Money(100), RoomStatus::Free, 4, {}, 40, 2, true, true, {&included});
    REQUIRE(suite.get_bedrooms() == 2);
    REQUIRE(suite.has_kitchen());
    REQUIRE(suite.has_jacuzzi());
    REQUIRE(suite.includes_service("breakfast"));
    REQUIRE_FALSE(suite.includes_service("spa"));
    REQUIRE(suite.calculate_price(2).value() == 800);
    REQUIRE(suite.price_for_stay(1, 25).value() == 300);
    REQUIRE_THROWS_AS(suite.calculate_price(0), InvalidDatesException);
}
