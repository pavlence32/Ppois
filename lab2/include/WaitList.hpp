#pragma once

#include "model.hpp"
#include <deque>

class Guest;
class GuestRequest;
class Room;
class Reservation;
class AvailabilityCalendar;
class BookingPolicy;

class WaitList
{
    std::deque<GuestRequest*> requests_;
    int max_size_;
    int notification_count_ = 0;
    bool notifications_enabled_ = true;

public:
    explicit WaitList(int max_size);
    void add(GuestRequest* request);
    void remove(GuestRequest* request);
    int position_of(Guest* guest) const;
    GuestRequest* find_matching(Room* room) const;
    Reservation& promote_next(Room* room, AvailabilityCalendar& calendar,
                               BookingPolicy* policy);
    void remove_expired(Date now);
    bool is_full() const;
    bool contains(Guest* guest) const;
    int size() const;
};
