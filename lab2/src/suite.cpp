#include "suite.hpp"

#include "exception.hpp"

Suite::Suite(int number, Floor* floor, Money base_price, RoomStatus status,
             int capacity, std::vector<Amenity> amenities, double area,
             int bedrooms, bool kitchen, bool jacuzzi,
             std::vector<Service*> included_services)
    : Room(number, floor, base_price, status, capacity, amenities, area),
      bedrooms_(bedrooms),
      kitchen_(kitchen),
      jacuzzi_(jacuzzi),
      included_services_(included_services)
{}

Money Suite::calculate_price(int nights) const
{
    if (nights <= 0)
        throw InvalidDatesException{"Количество ночей должно быть положительным"};
    return base_price() * bedrooms_ * 2.0 * nights;
}

int Suite::get_bedrooms() const
{
    return bedrooms_;
}

bool Suite::has_kitchen() const
{
    return kitchen_;
}

bool Suite::has_jacuzzi() const
{
    return jacuzzi_;
}

bool Suite::includes_service(const std::string& name) const
{
    for (Service* service : included_services_)
        if (service != nullptr && service->get_name() == name)
            return true;
    return false;
}

Money Suite::price_for_stay(int nights, double discount) const
{
    return calculate_price(nights).apply_discount(discount);
}
