#include "employee.hpp"
class Chef:public employee
{
    bool status=false;
    public:
    using employee::employee;
    void work();
    bool get_status();
    void set_status(bool a);
};