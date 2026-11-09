#include "guest.hpp"
Guest::Guest(int age,std::string name,std::string male,int number,std::string country,std::string city,std::string street,int house,int flat)
    : People(age,name,male,number), country(country), city(city), street(street), house(house), flat(flat)
{
}
std::string Guest::getCountry()
{
    return country;
}                                    
std::string Guest::getCity()
{
    return city;
}
std::string Guest::getStreet()
{
    return street;
}
int Guest::getHouse()
{
    return house;
}
int Guest::getFlat()
{
    return flat;
}

bool Guest::has_complete_address() const
{
    return !country.empty() && !city.empty() && !street.empty() &&
           house > 0 && flat >= 0;
}

std::string Guest::full_address() const
{
    return city + ", " + street + " " + std::to_string(house) +
           ", " + std::to_string(flat);
}

bool Guest::can_make_reservation() const
{
    return is_adult() && has_valid_name() && has_complete_address();
}