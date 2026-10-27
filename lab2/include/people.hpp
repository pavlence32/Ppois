#pragma once
#include <string>
class People
{
    int age;
    std::string name;
    std::string male;
    int number;
    public :
    People(int age,std::string name,std::string male,int number);
    int getAge() const;
    std::string getName() const;
    std::string getMale() const;
    int getNumber() const;



};