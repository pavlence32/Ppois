#pragma once

#include "rooms/room.hpp"
#include "services/service.hpp"

class Suite : public Room
{
    int bedrooms_;
    bool kitchen_;
    bool jacuzzi_;
    std::vector<Service*> included_services_;
    bool living_room_ = true;
    bool butler_service_ = false;
    int maximum_guests_ = 0;
    Money extra_bed_price_;

public:
    Suite(int number, Floor* floor, Money base_price, RoomStatus status,
          int capacity, std::vector<Amenity> amenities, double area,
          int bedrooms, bool kitchen, bool jacuzzi,
          std::vector<Service*> included_services);
    Money calculate_price(int nights) const override;
    int get_bedrooms() const;
    bool has_kitchen() const;
    bool has_jacuzzi() const;
    bool includes_service(const std::string& name) const;
    Money price_for_stay(int nights, double discount) const;
};
