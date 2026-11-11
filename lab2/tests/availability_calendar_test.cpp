#include <catch2/catch_test_macros.hpp>
#include "booking/AvailabilityCalendar.hpp"
#include "booking/Reservation.hpp"
#include "people/guest.hpp"
#include "rooms/standard_room.hpp"
#include "core/exception.hpp"

TEST_CASE("Availability calendar tracks reservations")
{
    Guest guest(30, "Ann", "F", 1, "BY", "Minsk", "Main", 1, 1);
    StandardRoom room(101, nullptr, Money(100), RoomStatus::Free, 2, {}, 20, false);
    AvailabilityCalendar calendar;
    auto from = now_date() + std::chrono::hours(24);
    auto to = from + std::chrono::hours(24);
    Reservation r("R", &guest, &room, from, to, 1);
    REQUIRE(calendar.is_free(&room, from, to));
    REQUIRE_FALSE(calendar.is_free(nullptr, from, to));
    REQUIRE_FALSE(calendar.is_free(&room, to, from));
    r.confirm(calendar);
    REQUIRE_FALSE(calendar.is_free(room, from, to));
    REQUIRE(calendar.find_free_rooms({&room}, from, to, 1).empty());
    REQUIRE(calendar.get_occupancy({&room}, from, to) == 100);
    REQUIRE(calendar.get_occupancy({}, from, to) == 0);
    REQUIRE_THROWS_AS(calendar.find_reservation("missing"), ReservationNotFoundException);
    REQUIRE_THROWS_AS(calendar.release(nullptr), ReservationNotFoundException);
    calendar.release(r);
    REQUIRE(calendar.is_free(&room, from, to) == false);
}
