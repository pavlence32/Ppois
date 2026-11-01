#include "service.hpp"

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
