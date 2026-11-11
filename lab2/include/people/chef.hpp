#pragma once

#include "people/employee.hpp"
class Chef:public employee
{
    bool status=false;
    int prepared_orders_ = 0;
    bool kitchen_access_ = true;
    public:
    using employee::employee;
    void work();
    bool get_status();
    void set_status(bool a);
};