#pragma once

#include <string>

class ParkingTicket
{
    std::string number_;
    std::string car_plate_;
    bool active_;

public:
    ParkingTicket(std::string number, std::string car_plate);
    void close();
    bool is_active() const;
    bool belongs_to_car(const std::string& plate) const;
};
