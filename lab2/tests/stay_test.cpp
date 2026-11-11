#include <catch2/catch_test_macros.hpp>
#include "booking/Stay.hpp"
#include "booking/Reservation.hpp"
#include "booking/AvailabilityCalendar.hpp"
#include "booking/BookingPolicy.hpp"
#include "people/guest.hpp"
#include "rooms/standard_room.hpp"
#include "hotel/hotel.hpp"
#include "core/exception.hpp"

TEST_CASE("Stay checks in and out")
{
    Guest guest(30, "Ann", "F", 1, "BY", "Minsk", "Main", 1, 1);
    StandardRoom room(101, nullptr, Money(100), RoomStatus::Free, 2, {}, 20, false);
    AvailabilityCalendar calendar;
    CancellationPolicy policy(2, 50);
    auto from = now_date() - std::chrono::hours(1);
    auto to = now_date() + std::chrono::hours(24);
    Reservation reservation("R", &guest, &room, from, to, 1, &policy);
    reservation.confirm(calendar);
    Stay stay(&reservation);
    REQUIRE_FALSE(stay.has_started());
    stay.check_in(now_date());
    REQUIRE(stay.has_started());
    REQUIRE(stay.status() == StayStatus::Active);
    REQUIRE(stay.key() != nullptr);
    REQUIRE(stay.current_price().value() == 100);
    REQUIRE_FALSE(stay.is_overdue(now_date()));
    Hotel hotel("H", 3, {"M", "S", "1"});
    REQUIRE_FALSE(stay.is_late_checkout(now_date(), hotel));
    stay.check_out(now_date());
    REQUIRE(stay.status() == StayStatus::Completed);
    REQUIRE(reservation.status() == ReservationStatus::Completed);
    REQUIRE(stay.actual_nights() == 1);
}

TEST_CASE("Stay detects late checkout and extends its key")
{
    Guest guest(30, "Ann", "F", 1, "BY", "Minsk", "Main", 1, 1);
    StandardRoom room(101, nullptr, Money(100), RoomStatus::Free, 2, {}, 20, false);
    AvailabilityCalendar calendar;
    CancellationPolicy policy(2, 50);
    auto from = now_date() - std::chrono::hours(1);
    auto to = now_date() + std::chrono::hours(24);
    Reservation reservation("R", &guest, &room, from, to, 1, &policy);
    reservation.confirm(calendar);
    Stay stay(&reservation);
    stay.check_in(now_date());
    Hotel hotel("H", 3, {"M", "S", "1"});
    auto late = to + std::chrono::hours(17);
    REQUIRE(stay.is_late_checkout(late, hotel));
    REQUIRE(stay.calculate_late_fee(late, hotel).is_positive());
    stay.extend_stay(to + std::chrono::hours(48), calendar);
    REQUIRE(reservation.check_out() == to + std::chrono::hours(48));
}

TEST_CASE("Stay rejects null reservation")
{
    REQUIRE_THROWS_AS(Stay(nullptr), InvalidReservationStateException);
}
