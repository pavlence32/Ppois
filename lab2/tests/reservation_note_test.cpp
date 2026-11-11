#include <catch2/catch_test_macros.hpp>
#include "booking/reservation_note.hpp"

TEST_CASE("Reservation note")
{
    ReservationNote note("late arrival", true);
    REQUIRE(note.is_important());
    REQUIRE_FALSE(note.is_empty());
    REQUIRE(note.was_created_before(now_date() + std::chrono::hours(1)));
    REQUIRE(ReservationNote("", false).is_empty());
}
