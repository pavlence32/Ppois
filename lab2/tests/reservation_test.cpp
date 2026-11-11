#include <catch2/catch_test_macros.hpp>
#include "booking/Reservation.hpp"
#include "booking/AvailabilityCalendar.hpp"
#include "booking/BookingPolicy.hpp"
#include "people/guest.hpp"
#include "rooms/standard_room.hpp"
#include "core/exception.hpp"

namespace {
struct Fixture {
    Guest guest{30, "Ann", "F", 1, "BY", "Minsk", "Main", 1, 1};
    StandardRoom room{101, nullptr, Money(100), RoomStatus::Free, 2, {}, 20, false};
    CancellationPolicy policy{2, 50.0};
    AvailabilityCalendar calendar;
    Date from = now_date() + std::chrono::hours(24);
    Date to = from + std::chrono::hours(48);
};
}

TEST_CASE("Reservation confirms and exposes data")
{
    Fixture f;
    Reservation r("R1", &f.guest, &f.room, f.from, f.to, 2, &f.policy);
    REQUIRE(r.id() == "R1");
    REQUIRE(r.guest() == &f.guest);
    REQUIRE(r.room() == &f.room);
    REQUIRE(r.nights() == 2);
    REQUIRE(r.status() == ReservationStatus::Pending);
    r.confirm(f.calendar);
    REQUIRE(r.status() == ReservationStatus::Confirmed);
    REQUIRE(r.is_active());
    REQUIRE(r.total_price().value() == 200);
    REQUIRE(r.price_per_night().value() == 100);
    REQUIRE(r.can_check_in(f.from));
    REQUIRE(r.overlaps(f.from, f.to));
    REQUIRE(r.can_cancel(now_date()));
    REQUIRE_FALSE(r.is_finished());
    REQUIRE(f.calendar.find_reservation("R1").id() == "R1");
    REQUIRE_THROWS_AS(f.calendar.find_reservation(1), ReservationNotFoundException);
}

TEST_CASE("Reservation supports extension, count changes and cancellation")
{
    Fixture f;
    Reservation r(7, &f.guest, &f.room, f.from, f.to, 1, &f.policy);
    r.confirm(f.calendar);
    r.change_guest_count(2);
    REQUIRE(r.guests_count() == 2);
    r.extend(f.to + std::chrono::hours(24), f.calendar);
    REQUIRE(r.nights() == 3);
    auto penalty = r.cancel(now_date());
    REQUIRE(penalty.is_positive());
    REQUIRE(r.status() == ReservationStatus::Cancelled);
    REQUIRE(r.is_finished());
}

TEST_CASE("Reservation rejects invalid transitions")
{
    Fixture f;
    Reservation r("R", &f.guest, &f.room, f.from, f.to, 1);
    REQUIRE_THROWS_AS(r.cancel(now_date()), CancellationNotAllowedException);
    REQUIRE_THROWS_AS(r.change_guest_count(0), RoomOverCapacityException);
    r.confirm(f.calendar);
    REQUIRE_THROWS_AS(r.confirm(f.calendar), InvalidReservationStateException);
}

TEST_CASE("Reservation marks no-show and rejects invalid extensions")
{
    Fixture f;
    Reservation r("R", &f.guest, &f.room, f.from, f.to, 1, &f.policy);
    REQUIRE_FALSE(r.can_check_in(now_date()));
    REQUIRE_THROWS_AS(r.mark_no_show(now_date()), InvalidReservationStateException);
    r.confirm(f.calendar);
    r.mark_no_show(f.to);
    REQUIRE(r.status() == ReservationStatus::NoShow);
    REQUIRE(r.is_finished());
    REQUIRE_FALSE(r.is_active());
    REQUIRE_FALSE(r.overlaps(f.from, f.to));
}
