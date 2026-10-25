#include "housekeeper.hpp"
void Housekeeper::work()
{
    std::cout<<getName()<<"убирает номер"<<std::endl;
}
bool Housekeeper::get_status()
{
    return status;
}
void Housekeeper::set_status(bool a)
{
    status=a;
}