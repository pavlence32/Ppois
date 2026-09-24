#include "algos.h"
int main()
{
    std::cout<<"Введите строку\n";
    std::string line;
    std::cin>>line;
    Markov a(line,"rules.txt");
    a.run();
    std::cout<<"Итог : "<<a.get_result()<<std::endl;
}