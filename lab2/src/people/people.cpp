#include "people/people.hpp"
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

bool People::is_adult() const
{
    return age >= 18;
}

bool People::has_valid_name() const
{
    return !name.empty();
}

bool People::same_person(const People& other) const
{
    return number == other.number;
}
