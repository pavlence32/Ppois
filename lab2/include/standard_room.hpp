#pragma once

#include "room.hpp"

class StandardRoom : public Room
{
    bool balcony_;

public:
    StandardRoom(int number, Floor* floor, Money base_price, RoomStatus status,
                 int capacity, std::vector<Amenity> amenities, double area,
                 bool balcony);
    Money calculate_price(int nights) const override;
    bool has_balcony() const;
};

using StandartRoom = StandardRoom;
