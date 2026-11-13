#pragma once

#include "people/people.hpp"
class Guest : public People
{
    std::string country;
    std::string city;
    std::string street;
    int house;
    int flat;
    std::string passport;
    std::string preferred_language;
    int loyalty_points = 0;
    bool vip = false;
    public :
    Guest(int age,std::string name,std::string male,int number,std::string country,std::string city,std::string street,int house,int flat);
    std::string getCountry();
    std::string getCity();
    std::string getStreet();
    int getHouse();
    int getFlat();
    bool has_complete_address() const;
    std::string full_address() const;
    bool can_make_reservation() const;
};