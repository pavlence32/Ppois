#include "algos.h"
int main()
{
    std::cout<<"Введите строку\n";
    std::string line;
    std::getline(std::cin, line);
    Markov a(line,Parser::parser());
    a.run();
    std::cout<<"Итог : "<<a.get_result()<<std::endl;
}