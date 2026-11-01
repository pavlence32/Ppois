#pragma once

#include "model.hpp"

#include <string>

class Guest;
class Room;
class AvailabilityCalendar;
class BookingPolicy;

class Reservation
{
    friend class Stay;
    std::string id_;
    Guest* guest_;
    Room* room_;
    Date check_in_;
    Date check_out_;
    int guests_count_;
    ReservationStatus status_;
    Money total_price_;
    Date created_at_;
    BookingPolicy* policy_;
    AvailabilityCalendar* calendar_;

public:
    Reservation(std::string id, Guest* guest, Room* room, Date check_in,
                Date check_out, int guests_count, BookingPolicy* policy = nullptr);
    Reservation(int id, Guest* guest, Room* room, Date check_in, Date check_out,
                int guests_count, BookingPolicy* policy = nullptr);

    const std::string& id() const;
    Guest* guest() const;
    Room* room() const;
    Date check_in() const;
    Date check_out() const;
    int guests_count() const;
    ReservationStatus status() const;
    Money total_price() const;
    Date created_at() const;
    BookingPolicy* policy() const;

    void confirm(AvailabilityCalendar& calendar);
    Money cancel(Date now);
    void extend(Date new_check_out, AvailabilityCalendar& calendar);
    int nights() const;
    Money calculate_total() const;
    bool overlaps(Date start, Date end) const;
    bool is_active() const;
    void mark_no_show(Date now);
};


