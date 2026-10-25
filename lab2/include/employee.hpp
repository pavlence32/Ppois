#include "people.hpp"
#include <iostream>
class employee: public People
{
    std::string position;
    public:
    employee(int age,std::string name,std::string male,int number,std::string position);
    void work();

};

