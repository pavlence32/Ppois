#include "hotel_event.hpp"

HotelEvent::HotelEvent(std::string name, Date starts_at, int capacity)
    : name_(name), starts_at_(starts_at), capacity_(capacity)
{}

bool HotelEvent::has_started(Date now) const { return now >= starts_at_; }
bool HotelEvent::can_accept(int guests) const
{
    return guests > 0 && guests <= capacity_;
}
bool HotelEvent::has_name() const { return !name_.empty(); }
