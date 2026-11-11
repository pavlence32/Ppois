#pragma once

#include "people/people.hpp"
#include "core/model.hpp"
#include <iostream>

class employee : public People
{
    std::string position;
    Money salary_;
    Date hired_at_;
    bool active_ = true;
    std::string shift_;
    public:
    employee(int age,std::string name,std::string male,int number,std::string position);
    void work();

};

using Employee = employee;
