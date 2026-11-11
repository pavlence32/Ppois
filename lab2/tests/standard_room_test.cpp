#include <catch2/catch_test_macros.hpp>
#include "rooms/standard_room.hpp"
#include "core/exception.hpp"

TEST_CASE("Standard room pricing")
{
    StandardRoom room(101, nullptr, Money(100), RoomStatus::Free, 2, {}, 20, true);
    REQUIRE(room.has_balcony());
    REQUIRE_FALSE(room.is_quiet_room());
    REQUIRE(room.calculate_price(1).value() == 100);
    REQUIRE(room.calculate_price(7).value() == 630);
    REQUIRE(room.price_for_stay(2, 10).value() == 180);
    REQUIRE_THROWS_AS(room.calculate_price(0), InvalidDatesException);
}
