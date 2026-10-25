#include "employee.hpp"
class receptionist:public employee
{
    bool status=true;
    public:
    using employee::employee;
    void work();
    void set_status(bool a);
    bool get_status();

};