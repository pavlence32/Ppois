#pragma once

#include "room.hpp"
#include "service.hpp"

class Suite : public Room
{
    int bedrooms_;
    bool kitchen_;
    bool jacuzzi_;
    std::vector<Service*> included_services_;

public:
    Suite(int number, Floor* floor, Money base_price, RoomStatus status,
          int capacity, std::vector<Amenity> amenities, double area,
          int bedrooms, bool kitchen, bool jacuzzi,
          std::vector<Service*> included_services);
    Money calculate_price(int nights) const override;
    int get_bedrooms() const;
    bool has_kitchen() const;
    bool has_jacuzzi() const;
};
