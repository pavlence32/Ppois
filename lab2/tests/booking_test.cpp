#include <catch2/catch_test_macros.hpp>

#include "core/exception.hpp"
#include "test_support.hpp"

TEST_CASE("Calendar prevents double booking and releases a room", "[booking]")
{
    support::BookingContext context;
    Reservation reservation = support::make_reservation(context);

    CHECK(context.calendar.is_free(&context.room,
                                   context.check_in, context.check_out));
    reservation.confirm(context.calendar);
    CHECK_FALSE(context.calendar.is_free(context.room,
                                         context.check_in, context.check_out));
    CHECK_THROWS_AS(context.calendar.book(&reservation), OverbookingException);

    context.calendar.release(reservation);
    CHECK_THROWS_AS(context.calendar.find_reservation("CLI-1"),
                    ReservationNotFoundException);
}

TEST_CASE("Calendar searches rooms by date and capacity", "[booking]")
{
    support::BookingContext context;
    StandardRoom small = support::make_room(102, 1);
    const std::vector<Room*> rooms{&context.room, &small};

    CHECK(context.calendar.find_free_rooms(
              rooms, context.check_in, context.check_out, 2).size() == 1);
    CHECK(context.calendar.get_occupancy({}, context.check_in,
                                         context.check_out) == 0);
    CHECK_FALSE(context.calendar.is_free(nullptr, context.check_in,
                                         context.check_out));
    CHECK_FALSE(context.calendar.is_free(&context.room, context.check_out,
                                         context.check_in));
}
#include <catch2/catch_test_macros.hpp>

#include "core/exception.hpp"
#include "test_support.hpp"

TEST_CASE("Cancellation policy gives a free cancellation window", "[booking]")
{
    support::BookingContext context;
    Reservation reservation = support::make_reservation(context);
    CancellationPolicy policy(2, 50.0);

    const Date free_day = context.check_in - std::chrono::hours(24 * 5);
    CHECK(policy.is_free_cancellation_period(reservation, free_day));
    CHECK(policy.calculate_penalty(reservation, free_day).is_zero());
    CHECK_FALSE(policy.has_penalty(reservation, free_day));
}

TEST_CASE("Cancellation policy charges a penalty near arrival", "[booking]")
{
    support::BookingContext context;
    Reservation reservation = support::make_reservation(context);
    reservation.confirm(context.calendar);
    const Date five_days_before = context.check_in - std::chrono::hours(24 * 5);
    const Date one_day_before = context.check_in - std::chrono::hours(24);

    CHECK(context.policy.is_free_cancellation_period(reservation,
                                                     five_days_before));
    CHECK_FALSE(context.policy.is_free_cancellation_period(reservation,
                                                           one_day_before));
    CHECK(context.policy.has_penalty(reservation, one_day_before));
    CHECK(context.policy.calculate_penalty(reservation, one_day_before)
              .value() == 100.0);
    CHECK(context.policy.get_refund_amount(reservation, one_day_before)
              .value() == 100.0);
}

TEST_CASE("Cancellation policy changes after arrival", "[booking]")
{
    support::BookingContext context;
    Reservation reservation = support::make_reservation(context);
    reservation.confirm(context.calendar);

    CHECK(context.policy.calculate_penalty(reservation, context.check_in)
              .value() == 100.0);
    CHECK(context.policy.get_refund_amount(reservation, context.check_in)
              .value() == 100.0);
}
#include <catch2/catch_test_macros.hpp>
#include "core/exception.hpp"
TEST_CASE("CancellationNotAllowedException stores message") { CancellationNotAllowedException e("x"); REQUIRE(e.getMessage() == "x"); }
#include <catch2/catch_test_macros.hpp>

#include "booking/GuestRequest.hpp"
#include "core/exception.hpp"
#include "test_support.hpp"

TEST_CASE("Guest request finds a suitable room", "[booking]")
{
    Guest guest = support::make_guest();
    StandardRoom room = support::make_room();
    room.add_amenity(Amenity("wifi", AmenityCategory::Technology, false));

    GuestRequest request(&guest, support::days_after(1),
                         support::days_after(3), 2, {"wifi"}, Money(100));

    REQUIRE(request.status() == RequestStatus::Open);
    CHECK(request.matches(&room));
    CHECK(request.needs_amenity("wifi"));
    CHECK(request.accepts_price(Money(100)));
    CHECK(request.nights() == 2);

    request.fulfill();
    CHECK(request.status() == RequestStatus::Fulfilled);
    CHECK_THROWS_AS(request.cancel(), InvalidReservationStateException);
}

TEST_CASE("Guest request is rejected when room cannot satisfy it", "[booking]")
{
    Guest guest = support::make_guest();
    StandardRoom room = support::make_room(101, 1);
    GuestRequest request(&guest, support::days_after(1),
                         support::days_after(2), 2, {"missing"}, Money(50));

    CHECK_FALSE(request.matches(&room));
    CHECK_FALSE(request.matches(nullptr));
    request.cancel();
    CHECK(request.status() == RequestStatus::Fulfilled);
}

TEST_CASE("Guest request rejects an invalid date range", "[booking]")
{
    Guest guest = support::make_guest();
    const Date date = support::today();
    CHECK_THROWS_AS(GuestRequest(&guest, date, date, 1, {}, Money(10)),
                    InvalidDatesException);
}
#include <catch2/catch_test_macros.hpp>
#include "core/exception.hpp"
TEST_CASE("InvalidDateException stores message") { InvalidDateException e("x"); REQUIRE(e.getMessage() == "x"); }
#include <catch2/catch_test_macros.hpp>
#include "core/exception.hpp"
TEST_CASE("InvalidDatesException stores message") { InvalidDatesException e("x"); REQUIRE(e.getMessage() == "x"); }
#include <catch2/catch_test_macros.hpp>
#include "core/exception.hpp"
TEST_CASE("InvalidReservationStateException stores message") { InvalidReservationStateException e("x"); REQUIRE(e.getMessage() == "x"); }
#include <catch2/catch_test_macros.hpp>
#include "core/exception.hpp"
TEST_CASE("OverbookingException stores message") { OverbookingException e("x"); REQUIRE(e.getMessage() == "x"); }
#include <catch2/catch_test_macros.hpp>
#include "core/exception.hpp"
TEST_CASE("ReservationNotFoundException stores message") { ReservationNotFoundException e("x"); REQUIRE(e.getMessage() == "x"); }
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
#include <catch2/catch_test_macros.hpp>

#include "core/exception.hpp"
#include "test_support.hpp"

TEST_CASE("Guest creates, confirms and changes a reservation", "[booking]")
{
    support::BookingContext context;
    Reservation reservation = support::make_reservation(context, 2);

    REQUIRE(reservation.status() == ReservationStatus::Pending);
    REQUIRE(reservation.nights() == 2);

    reservation.confirm(context.calendar);

    CHECK(reservation.status() == ReservationStatus::Confirmed);
    CHECK(reservation.is_active());
    CHECK(reservation.total_price().value() == 200);
    CHECK(context.room.get_status() == RoomStatus::Reserved);
    CHECK(context.calendar.find_reservation("CLI-1").guest() == &context.guest);

    reservation.change_guest_count(1);
    CHECK(reservation.guests_count() == 1);
    CHECK(reservation.can_check_in(context.check_in));
}

TEST_CASE("Guest extends a reservation and cancels it", "[booking]")
{
    support::BookingContext context;
    Reservation reservation = support::make_reservation(context);
    reservation.confirm(context.calendar);

    reservation.extend(support::days_after(5), context.calendar);
    CHECK(reservation.nights() == 3);

    const Money penalty = reservation.cancel(support::today());
    CHECK(penalty.is_positive());
    CHECK(reservation.status() == ReservationStatus::Cancelled);
    CHECK(reservation.is_finished());
    CHECK(context.room.get_status() == RoomStatus::Free);
}

TEST_CASE("Reservation rejects invalid booking workflow steps", "[booking]")
{
    support::BookingContext context;
    Reservation reservation = support::make_reservation(context);

    CHECK_THROWS_AS(reservation.change_guest_count(0),
                    RoomOverCapacityException);
    CHECK_THROWS_AS(reservation.extend(context.check_out, context.calendar),
                    InvalidReservationStateException);

    reservation.confirm(context.calendar);
    CHECK_THROWS_AS(reservation.confirm(context.calendar),
                    InvalidReservationStateException);
}

TEST_CASE("Reservation can end as a no-show", "[booking]")
{
    support::BookingContext context;
    Reservation reservation = support::make_reservation(context);
    reservation.confirm(context.calendar);

    reservation.mark_no_show(context.check_in + std::chrono::hours(24));
    CHECK(reservation.status() == ReservationStatus::NoShow);
    CHECK_FALSE(reservation.is_active());
    CHECK(reservation.is_finished());
}
#include <catch2/catch_test_macros.hpp>

#include "booking/Stay.hpp"
#include "core/exception.hpp"
#include "rooms/room_key.hpp"
#include "test_support.hpp"

TEST_CASE("Guest completes a hotel stay", "[hotel][booking]")
{
    support::BookingContext context;
    Reservation reservation = support::make_reservation(context);
    reservation.confirm(context.calendar);
    Stay stay(&reservation);

    REQUIRE_FALSE(stay.has_started());
    stay.check_in(context.check_in);

    CHECK(stay.has_started());
    CHECK(stay.status() == StayStatus::Active);
    CHECK(stay.key() != nullptr);
    CHECK(reservation.status() == ReservationStatus::CheckedIn);
    CHECK(context.room.get_status() == RoomStatus::Occupied);

    Hotel hotel("Пример", 3, {"Минск", "Центральная", "1"});
    CHECK_FALSE(stay.is_late_checkout(context.check_in, hotel));
    stay.check_out(context.check_in + std::chrono::hours(24));

    CHECK(stay.status() == StayStatus::Completed);
    CHECK(reservation.status() == ReservationStatus::Completed);
    CHECK(context.room.get_status() == RoomStatus::Dirty);
    CHECK(stay.actual_nights() >= 1);
}

TEST_CASE("Guest requests late checkout and extends stay", "[hotel][booking]")
{
    support::BookingContext context;
    Reservation reservation = support::make_reservation(context);
    reservation.confirm(context.calendar);
    Stay stay(&reservation);
    stay.check_in(context.check_in);

    Hotel hotel("Пример", 3, {"Минск", "Центральная", "1"});
    const Date late = context.check_out + std::chrono::hours(17);
    CHECK(stay.is_late_checkout(late, hotel));
    CHECK(stay.calculate_late_fee(late, hotel).is_positive());

    const Date extended_checkout = support::days_after(6);
    stay.extend_stay(extended_checkout, context.calendar);
    CHECK(reservation.check_out() == extended_checkout);
    CHECK(stay.key()->is_valid(context.check_in));
}

TEST_CASE("Stay refuses to start without a reservation", "[hotel]")
{
    CHECK_THROWS_AS(Stay(nullptr), InvalidReservationStateException);
}
#include <catch2/catch_test_macros.hpp>
#include "core/exception.hpp"
TEST_CASE("WaitListFullException stores message") { WaitListFullException e("x"); REQUIRE(e.getMessage() == "x"); }
#include <catch2/catch_test_macros.hpp>

#include "booking/GuestRequest.hpp"
#include "booking/WaitList.hpp"
#include "core/exception.hpp"
#include "test_support.hpp"

TEST_CASE("Wait list promotes the next suitable guest", "[booking]")
{
    support::BookingContext context;
    GuestRequest request(&context.guest, context.check_in,
                         context.check_out, 1, {}, Money(100));
    WaitList wait_list(2);
    wait_list.add(&request);

    CHECK(wait_list.contains(&context.guest));
    CHECK(wait_list.position_of(&context.guest) == 1);
    CHECK(wait_list.find_matching(&context.room) == &request);

    Reservation& reservation = wait_list.promote_next(
        &context.room, context.calendar, &context.policy);
    CHECK(reservation.status() == ReservationStatus::Confirmed);
    CHECK(request.status() == RequestStatus::Fulfilled);
    CHECK(wait_list.size() == 0);
    delete &reservation;
}

TEST_CASE("Wait list removes expired requests and reports capacity", "[booking]")
{
    support::BookingContext context;
    GuestRequest request(&context.guest, context.check_in,
                         context.check_out, 1, {}, Money(100));
    WaitList wait_list(1);
    wait_list.add(&request);

    CHECK(wait_list.is_full());
    CHECK_THROWS_AS(wait_list.add(&request), WaitListFullException);
    wait_list.remove_expired(context.check_in);
    CHECK(wait_list.size() == 0);
    CHECK_FALSE(wait_list.contains(&context.guest));
}

TEST_CASE("Wait list reports when no guest matches a room", "[booking]")
{
    support::BookingContext context;
    GuestRequest request(&context.guest, context.check_in,
                         context.check_out, 2, {}, Money(10));
    WaitList wait_list(1);
    wait_list.add(&request);

    CHECK(wait_list.find_matching(&context.room) == nullptr);
    CHECK_THROWS_AS(wait_list.promote_next(&context.room, context.calendar,
                                           &context.policy),
                    ReservationNotFoundException);
}
