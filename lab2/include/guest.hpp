#include "people.hpp"
class Guest : public People
{
    std::string country;
    std::string city;
    std::string street;
    int house;
    int flat;
    public :
    Guest(int age,std::string name,std::string male,int number,std::string country,std::string city,std::string street,int house,int flat);
    std::string getCountry();
    std::string getCity();
    std::string getStreet();
    int getHouse();
    int getFlat();
};