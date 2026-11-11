#pragma once

#include "booking/AvailabilityCalendar.hpp"
#include "booking/BookingPolicy.hpp"
#include "booking/Reservation.hpp"
#include "hotel/hotel.hpp"
#include "people/guest.hpp"
#include "rooms/standard_room.hpp"

#include <chrono>

namespace support
{
inline Date today()
{
    return now_date();
}

inline Date days_after(int days)
{
    return today() + std::chrono::hours(24 * days);
}

inline Guest make_guest(int number = 1)
{
    return Guest(30, "Иван Иванов", "м", number, "Беларусь",
                 "Минск", "Ленина", 10, 5);
}

inline StandardRoom make_room(int number = 101, int capacity = 2,
                              RoomStatus status = RoomStatus::Free)
{
    return StandardRoom(number, nullptr, Money(100), status, capacity, {},
                        24.0, false);
}

struct BookingContext
{
    Guest guest = make_guest();
    StandardRoom room = make_room();
    CancellationPolicy policy{2, 50.0};
    AvailabilityCalendar calendar;
    Date check_in = days_after(2);
    Date check_out = days_after(4);
};

inline Reservation make_reservation(BookingContext& context,
                                    int guests = 1)
{
    return Reservation("CLI-1", &context.guest, &context.room,
                       context.check_in, context.check_out, guests,
                       &context.policy);
}
}
