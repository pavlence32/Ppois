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
    int getAge();
    std::string getName();
    std::string getMale();
    int getNumber();



};