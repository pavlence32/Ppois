#pragma once

#include "room.hpp"

class StandardRoom : public Room
{
    bool balcony_;
    bool breakfast_available_ = true;
    int cleaning_interval_ = 1;
    bool accessible_ = false;

public:
    StandardRoom(int number, Floor* floor, Money base_price, RoomStatus status,
                 int capacity, std::vector<Amenity> amenities, double area,
                 bool balcony);
    Money calculate_price(int nights) const override;
    bool has_balcony() const;
    bool is_quiet_room() const;
    Money price_for_stay(int nights, double discount) const;
};

using StandartRoom = StandardRoom;
