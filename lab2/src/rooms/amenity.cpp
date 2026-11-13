#include "rooms/amenity.hpp"

#include "core/exception.hpp"

Amenity::Amenity(std::string name, AmenityCategory category, bool paid,
                 Money price_per_day)
    : name_(name),
      category_(category),
      paid_(paid),
      price_per_day_(price_per_day),
      usable_(true)
{}

const std::string& Amenity::get_name() const
{
    return name_;
}

Money Amenity::get_cost(int nights) const
{
    if (nights < 0)
        throw InvalidDatesException{"Количество ночей не может быть отрицательным"};
    if (!paid_)
        return Money{};
    return price_per_day_ * nights;
}

void Amenity::mark_broken()
{
    usable_ = false;
}

void Amenity::repair()
{
    usable_ = true;
}

bool Amenity::is_usable() const
{
    return usable_;
}

bool Amenity::is_paid() const
{
    return paid_;
}

void Amenity::change_price(Money price)
{
    price_per_day_ = price;
}

bool Amenity::is_available_for(int nights) const
{
    return usable_ && nights > 0;
}
