#include "hotel/parking_ticket.hpp"

ParkingTicket::ParkingTicket(std::string number, std::string car_plate)
    : number_(number), car_plate_(car_plate), active_(true)
{}

void ParkingTicket::close() { active_ = false; }
bool ParkingTicket::is_active() const { return active_; }
bool ParkingTicket::belongs_to_car(const std::string& plate) const
{
    return car_plate_ == plate;
}
