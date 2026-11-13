#include "services/service.hpp"

Service::Service(std::string name, Money price)
    : name_(name), price_(price)
{}

const std::string& Service::get_name() const
{
    return name_;
}

Money Service::get_price() const
{
    return price_;
}

Money Service::price_for(int count) const
{
    if (count < 0)
        return Money{};
    return price_ * count;
}

Money Service::price_with_discount(int count, double rate) const
{
    return price_for(count).apply_discount(rate);
}

bool Service::is_free() const
{
    return price_.is_zero();
}
