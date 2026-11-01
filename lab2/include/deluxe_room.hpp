#pragma once

#include "room.hpp"

class DeluxeRoom : public Room
{
    bool balcony_;
    ViewType view_;

public:
    DeluxeRoom(int number, Floor* floor, Money base_price, RoomStatus status,
               int capacity, std::vector<Amenity> amenities, double area,
               bool balcony, ViewType view);
    Money calculate_price(int nights) const override;
    bool has_balcony() const;
    ViewType get_view() const;
};
