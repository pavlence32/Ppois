#include <catch2/catch_test_macros.hpp>
#include "booking/GuestRequest.hpp"
#include "people/guest.hpp"
#include "rooms/standard_room.hpp"
#include "core/exception.hpp"

TEST_CASE("Guest request matches and changes state")
{
    Guest guest(30, "Ann", "F", 1, "BY", "Minsk", "Main", 1, 1);
    StandardRoom room(101, nullptr, Money(100), RoomStatus::Free, 2, {}, 20, false);
    auto from = now_date() + std::chrono::hours(24);
    auto to = from + std::chrono::hours(48);
    Amenity wifi("wifi", AmenityCategory::Technology, false);
    room.add_amenity(wifi);
    GuestRequest request(&guest, from, to, 2, {"wifi"}, Money(100));
    REQUIRE(request.guest() == &guest);
    REQUIRE(request.nights() == 2);
    REQUIRE(request.matches(&room));
    REQUIRE_FALSE(request.matches(nullptr));
    REQUIRE(request.accepts_price(Money(100)));
    REQUIRE_FALSE(request.accepts_price(Money(101)));
    REQUIRE(request.needs_amenity("wifi"));
    REQUIRE_FALSE(request.is_expired(now_date()));
    REQUIRE(request.is_expired(from));
    request.fulfill();
    REQUIRE(request.status() == RequestStatus::Fulfilled);
    REQUIRE_THROWS_AS(request.cancel(), InvalidReservationStateException);
}

TEST_CASE("Guest request rejects capacity, amenities and price")
{
    Guest guest(30, "Ann", "F", 1, "BY", "Minsk", "Main", 1, 1);
    StandardRoom room(101, nullptr, Money(100), RoomStatus::Free, 1, {}, 20, false);
    auto from = now_date() + std::chrono::hours(24);
    GuestRequest request(&guest, from, from + std::chrono::hours(24), 2,
                         {"missing"}, Money(50));
    REQUIRE_FALSE(request.matches(&room));
    request.cancel();
    REQUIRE(request.status() == RequestStatus::Fulfilled);
}

TEST_CASE("Guest request rejects invalid data")
{
    Guest guest(30, "Ann", "F", 1, "BY", "Minsk", "Main", 1, 1);
    const Date date = now_date();
    REQUIRE_THROWS_AS(GuestRequest(&guest, date, date, 1, {}, Money(10)),
                      InvalidDatesException);
}
