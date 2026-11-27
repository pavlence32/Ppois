#include "core/Address.hpp"

#include <sstream>

Address::Address(std::string street,
                 std::string city,
                 std::string postal_code,
                 std::string country,
                 std::string building)
    : street_(std::move(street)),
      city_(std::move(city)),
      postal_code_(std::move(postal_code)),
      country_(std::move(country)),
      building_(std::move(building))
{
}

std::string Address::format() const
{
    std::ostringstream stream;
    stream << country_ << ", " << city_ << ", " << street_ << " " << building_
           << " (" << postal_code_ << ")";
    return stream.str();
}

bool Address::same_city(const Address& other) const
{
    return city_ == other.city_ && country_ == other.country_;
}

bool Address::is_complete() const
{
    return !street_.empty() && !city_.empty() && !postal_code_.empty()
        && !country_.empty() && !building_.empty();
}

std::string Address::city() const
{
    return city_;
}
