#include "standard_room.hpp"

#include "exception.hpp"

StandardRoom::StandardRoom(int number, Floor* floor, Money base_price,
                           RoomStatus status, int capacity,
                           std::vector<Amenity> amenities, double area,
                           bool balcony)
    : Room(number, floor, base_price, status, capacity, amenities, area),
      balcony_(balcony)
{}

Money StandardRoom::calculate_price(int nights) const
{
    if (nights <= 0)
        throw InvalidDatesException{"Количество ночей должно быть положительным"};
    if (nights >= 7)
        return base_price() * nights * 0.9;
    return base_price() * nights;
}

bool StandardRoom::has_balcony() const
{
    return balcony_;
}
