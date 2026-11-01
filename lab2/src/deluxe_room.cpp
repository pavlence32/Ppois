#include "deluxe_room.hpp"

#include "exception.hpp"

DeluxeRoom::DeluxeRoom(int number, Floor* floor, Money base_price,
                       RoomStatus status, int capacity,
                       std::vector<Amenity> amenities, double area,
                       bool balcony, ViewType view)
    : Room(number, floor, base_price, status, capacity, amenities, area),
      balcony_(balcony),
      view_(view)
{}

Money DeluxeRoom::calculate_price(int nights) const
{
    if (nights <= 0)
        throw InvalidDatesException{"Количество ночей должно быть положительным"};
    double view_surcharge = 0.0;
    if (view_ == ViewType::Sea)
        view_surcharge = 50.0;
    else if (view_ == ViewType::Park)
        view_surcharge = 20.0;
    return base_price() * 1.5 * nights + Money(view_surcharge);
}

bool DeluxeRoom::has_balcony() const
{
    return balcony_;
}

ViewType DeluxeRoom::get_view() const
{
    return view_;
}
