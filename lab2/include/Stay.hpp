#pragma once

#include "model.hpp"

class Reservation;
class RoomKey;
class AvailabilityCalendar;
class Hotel;

class Stay
{
    Reservation* reservation_;
    RoomKey* key_;
    Date actual_check_in_;
    Date actual_check_out_;
    StayStatus status_;

public:
    explicit Stay(Reservation* reservation);
    void check_in(Date now);
    void check_out(Date now);
    bool is_late_checkout(Date now, const Hotel& hotel) const;
    Money calculate_late_fee(Date now, const Hotel& hotel) const;
    int actual_nights() const;
    void extend_stay(Date new_date, AvailabilityCalendar& calendar);
    Reservation* reservation() const;
    RoomKey* key() const;
    StayStatus status() const;
};
