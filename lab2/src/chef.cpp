#include "chef.hpp"
void Chef::work()
{
    std::cout<<getName()<<"готовит"<<std::endl;
}
bool Chef::get_status()
{
    return status;
}
void Chef::set_status(bool a)
{
    status=a;
}