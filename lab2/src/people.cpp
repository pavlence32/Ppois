#include "people.hpp"
People::People(int age, std::string name, std::string male, int number)
    : age(age), name(name), male(male), number(number)
{
}
int People::getAge()
{
    return age;
}
std::string People::getName()
{
    return name;
}
std::string People::getMale()
{
    return male;
}
int People::getNumber()
{
    return number;
}
