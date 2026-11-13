#pragma once
#include <string>
class People
{
    int age;
    std::string name;
    std::string male;
    int number;
    std::string phone;
    std::string email;
    std::string document_number;
    bool verified = false;
    public :
    People(int age,std::string name,std::string male,int number);
    int getAge() const;
    std::string getName() const;
    std::string getMale() const;
    int getNumber() const;
    bool is_adult() const;
    bool has_valid_name() const;
    bool same_person(const People& other) const;



};