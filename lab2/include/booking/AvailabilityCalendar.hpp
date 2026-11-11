#pragma once

#include "core/model.hpp"

#include <map>
#include <vector>

class Room;
class Reservation;

class AvailabilityCalendar
{
    std::map<Room*, std::vector<Reservation*>> bookings_;
    Date last_update_;
    bool automatic_updates_ = true;
    int change_count_ = 0;

public:
    bool is_free(Room* room, Date start, Date end) const;
    bool is_free(Room& room, Date start, Date end) const
    {
        return is_free(&room, start, end);
    }
    void book(Reservation* reservation);
    void book(Reservation& reservation) { book(&reservation); }
    void release(Reservation* reservation);
    void release(Reservation& reservation) { release(&reservation); }
    Reservation& find_reservation(const std::string& id) const;
    Reservation& find_reservation(int id) const;
    std::vector<Room*> find_free_rooms(const std::vector<Room*>& rooms,
                                        Date start, Date end, int guests) const;
    double get_occupancy(const std::vector<Room*>& rooms, Date start, Date end) const;
};
