#include <catch2/catch_test_macros.hpp>
#include "rooms/deluxe_room.hpp"
#include "core/exception.hpp"

TEST_CASE("Deluxe room views and pricing")
{
    DeluxeRoom sea(201, nullptr, Money(100), RoomStatus::Free, 2, {}, 25, true, ViewType::Sea);
    DeluxeRoom city(202, nullptr, Money(100), RoomStatus::Free, 2, {}, 25, false, ViewType::City);
    DeluxeRoom park(203, nullptr, Money(100), RoomStatus::Free, 2, {}, 25, false, ViewType::Park);
    REQUIRE(sea.has_balcony());
    REQUIRE(sea.get_view() == ViewType::Sea);
    REQUIRE(sea.has_premium_view());
    REQUIRE(city.get_view() == ViewType::City);
    REQUIRE_FALSE(city.has_premium_view());
    REQUIRE(sea.calculate_price(1).value() == 200);
    REQUIRE(park.calculate_price(1).value() == 170);
    REQUIRE(sea.price_for_stay(2, 10).value() == 315);
    REQUIRE_THROWS_AS(city.calculate_price(0), InvalidDatesException);
}
