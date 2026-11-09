#pragma once

#include "room.hpp"

class DeluxeRoom : public Room
{
    bool balcony_;
    ViewType view_;
    bool minibar_ = false;
    bool room_service_ = true;
    int floor_service_level_ = 1;

public:
    DeluxeRoom(int number, Floor* floor, Money base_price, RoomStatus status,
               int capacity, std::vector<Amenity> amenities, double area,
               bool balcony, ViewType view);
    Money calculate_price(int nights) const override;
    bool has_balcony() const;
    ViewType get_view() const;
    bool has_premium_view() const;
    Money price_for_stay(int nights, double discount) const;
};
