#include <catch2/catch_test_macros.hpp>
#include "booking/WaitList.hpp"
#include "booking/GuestRequest.hpp"
#include "booking/AvailabilityCalendar.hpp"
#include "booking/BookingPolicy.hpp"
#include "booking/Reservation.hpp"
#include "people/guest.hpp"
#include "rooms/standard_room.hpp"
#include "core/exception.hpp"

TEST_CASE("Wait list stores and removes requests")
{
    Guest guest(30, "Ann", "F", 1, "BY", "Minsk", "Main", 1, 1);
    auto from = now_date() + std::chrono::hours(24);
    GuestRequest request(&guest, from, from + std::chrono::hours(24), 1, {}, Money(100));
    WaitList list(1);
    list.add(&request);
    REQUIRE(list.size() == 1);
    REQUIRE(list.is_full());
    REQUIRE(list.contains(&guest));
    REQUIRE(list.position_of(&guest) == 1);
    REQUIRE_THROWS_AS(list.add(&request), WaitListFullException);
    list.remove_expired(from);
    REQUIRE(list.size() == 0);
    REQUIRE_FALSE(list.contains(&guest));
}

TEST_CASE("Wait list rejects invalid capacity")
{
    REQUIRE_THROWS_AS(WaitList(-1), WaitListFullException);
}

TEST_CASE("Wait list promotes a matching request")
{
    Guest guest(30, "Ann", "F", 1, "BY", "Minsk", "Main", 1, 1);
    StandardRoom room(101, nullptr, Money(100), RoomStatus::Free, 2, {}, 20, false);
    auto from = now_date() + std::chrono::hours(24);
    GuestRequest request(&guest, from, from + std::chrono::hours(24), 1, {}, Money(100));
    WaitList list(2);
    AvailabilityCalendar calendar;
    CancellationPolicy policy(2, 50);
    list.add(&request);
    REQUIRE(list.find_matching(&room) == &request);
    Reservation& reservation = list.promote_next(&room, calendar, &policy);
    REQUIRE(reservation.status() == ReservationStatus::Confirmed);
    REQUIRE(list.size() == 0);
    delete &reservation;
}

TEST_CASE("Wait list returns no match")
{
    Guest guest(30, "Ann", "F", 1, "BY", "Minsk", "Main", 1, 1);
    StandardRoom room(101, nullptr, Money(100), RoomStatus::Free, 2, {}, 20, false);
    auto from = now_date() + std::chrono::hours(24);
    GuestRequest request(&guest, from, from + std::chrono::hours(24), 1, {}, Money(50));
    WaitList list(1);
    list.add(&request);
    REQUIRE(list.find_matching(&room) == nullptr);
    AvailabilityCalendar calendar;
    REQUIRE_THROWS_AS(list.promote_next(&room, calendar, nullptr),
                      ReservationNotFoundException);
}
