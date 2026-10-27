#include "people.hpp"
People::People(int age, std::string name, std::string male, int number)
    : age(age), name(name), male(male), number(number)
{
}
int People::getAge() const
{
    return age;
}
std::string People::getName() const
{
    return name;
}
std::string People::getMale() const
{
    return male;
}
int People::getNumber() const 
{
    return number;
}
