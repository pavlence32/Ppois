#pragma once

#include "employee.hpp"
class Housekeeper:public employee
{
    bool status=false;
    int cleaned_rooms_ = 0;
    bool has_master_key_ = false;
    public:
    using employee::employee;
    void work();
    bool get_status();
    void set_status(bool a);
};