#include <catch2/catch_test_macros.hpp>
#include "booking/BookingPolicy.hpp"
#include "booking/Reservation.hpp"
#include "booking/AvailabilityCalendar.hpp"
#include "people/guest.hpp"
#include "rooms/standard_room.hpp"
#include "core/exception.hpp"

TEST_CASE("Cancellation policy calculates free and paid cancellation")
{
    Guest guest(30, "Ann", "F", 1, "BY", "Minsk", "Main", 1, 1);
    StandardRoom room(101, nullptr, Money(100), RoomStatus::Free, 2, {}, 20, false);
    AvailabilityCalendar calendar;
    auto from = now_date() + std::chrono::hours(24 * 5);
    Reservation r("R", &guest, &room, from, from + std::chrono::hours(48), 1);
    CancellationPolicy policy(2, 50.0);
    REQUIRE(policy.is_free_cancellation_period(r, now_date()));
    r.confirm(calendar);
    REQUIRE(policy.days_before_arrival(r, now_date()) >= 4);
    REQUIRE(policy.calculate_penalty(r, now_date()).is_zero());
    REQUIRE_FALSE(policy.has_penalty(r, now_date()));
    REQUIRE(policy.calculate_refund(r, now_date()).value() == r.total_price().value());
}

TEST_CASE("Cancellation policy rejects invalid parameters")
{
    REQUIRE_THROWS_AS(CancellationPolicy(-1, 10), InvalidPaymentException);
    REQUIRE_THROWS_AS(CancellationPolicy(1, 101), InvalidPaymentException);
}
