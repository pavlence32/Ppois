#include "receptionist.hpp"
void receptionist::work()
{
std::cout<<"Вас обслуживает администратор "<<getName()<<std::endl;
}
void receptionist::set_status(bool a)
{
	status = a;
}
bool receptionist::get_status()
{
    return status;
}
