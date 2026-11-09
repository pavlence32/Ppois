#pragma once

#include "employee.hpp"
class receptionist:public employee
{
    bool status=true;
    bool can_check_in_guests_ = true;
    int served_guests_ = 0;
    public:
    using employee::employee;
    void work();
    void set_status(bool a);
    bool get_status();

};