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