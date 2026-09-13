#include "kubik.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
void menu()
{
    std::cout << "1-Загрузка с файла\n"
              << "2-Поворот грани\n"
              << "3-Проверка на сборку\n"
              << "4-Рандомное заполнение\n"
              << "5-Просмотр текущего состояние\n"
              << "6-Выход\n";
}
int main()
{
    int choose;
    Kubick kub;
    while (true)
    {
        menu();
        std::cin >> choose;
        switch (choose)
        {
        case 1:
        {
            std::ifstream inf("Sometxt.txt");
            if (!inf)
            {
                std::cout << "Ошибка открытия файла\n";
                break;
            }
            kub.load_color(inf);
            break;
        }
        case 2:
        {
            int a;
            std::cout << "Введите номер грани\n";
            std::cin >> a;
            if (a < 0 || a > 5)
            {
                std::cout << "Неверный индекс грани\n";
                break;
            }
            int choose;
            std::cout << "1- ->\n0- <-\n";
            std::cin >> choose;
            if(choose!=1 && choose !=0)
            {std::cout<<"Некорректный выбор\n";break;}
            kub.turn(a,choose);
            break;
        }
        case 3:
            if(kub.checkcolorplace())
            std::cout<<"Кубик собран\n";
            else std::cout<<"Кубик не собран\n";
            break;
        case 4:
            kub.random_place();
            break;
        case 5:
        std::cout<<kub<<std::endl;
        break;
        case 6:
        return 0;
        default:
            std::cout << "Неверно введенный номер\n";
            break;
        }
    }
    return 0;
}