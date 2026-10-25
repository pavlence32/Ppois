#include "employee.hpp"
#include <iostream>
employee::employee(int age,std::string name,std::string male,int number,std::string position) : People(age,name,male,number),position{position}
{};
void employee::work()
{
    std::cout << "Employee " << getName() << " is working." << std::endl;
}   

