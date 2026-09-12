#include "kubik.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
void Kubick::get_index(int * arr,int size,int a)
{
    switch (a)
    {
        case 0:
        arr[0]=2;
        arr[1]=3;
        arr[2]=4;
        arr[3]=5;
        case 1:
        arr[0]=2;
        arr[1]=3;
        arr[2]=4;
        arr[3]=5;
        case 2:
        arr[0]=0;
        arr[1]=3;
        arr[2]=1;
        arr[3]=5;
        case 3:
        arr[0]=0;
        arr[1]=1;
        arr[2]=4;
        arr[3]=5;
        case 4:
        arr[0]=0;
        arr[1]=5;
        arr[2]=1;
        arr[3]=5;
        case 5:
        arr[0]=0;
        arr[1]=2;
        arr[2]=1;
        arr[3]=4;

    }
}
char Kubick::fill_place(int index)
{
    switch (index)
    {
    case 0:
        return 'W';
    case 1:
        return 'Y';
    case 2:
        return 'R';
    case 3:
        return 'B';
    case 4:
        return 'O';
    case 5:
        return 'G';
    }
}
Kubick::Kubick()
{
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                faces[i][j][k] = Kubick::fill_place(i);
            }
        }
    }
}
void Kubick::load_color(std::string s)
{
    std::ifstream inf(s);
    if (!inf)
    {
        std::cout << "Ошибка открытия файла\n";
        return;
    }

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                char c;
                if (!(inf >> c))
                {
                    std::cout << "В файле недостаточно записей\n";
                    return;
                }
                faces[i][j][k] = c;
            }
        }
    }
}
void Kubick::random_place()
{
    srand(time(NULL));
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                int random_index = rand() % 6;
                faces[i][j][k] = fill_place(random_index);
            }
        }
    }
}
bool Kubick::checkcolorplace()
{
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                if (faces[i][j][k] != fill_place(i))
                {
                    std::cout << "Кубик собран не до конца\n";
                    return false;
                }
            }
        }
    }
    return true;
}
void Kubick::get_row(int f,int s,char * out)
{
    for (int i=0;i<3;i++)
    out[i]=faces[f][s][i];
}
void Kubick::set_row(int f,int s,char * out)
{
    for (int i=0;i<3;i++)
    faces[f][s][i]=out[i];
}
void Kubick::get_col(int f,int s,char * out)
{
    for (int i=0;i<3;i++)
    out[i]=faces[f][i][s];
}
void Kubick::set_col(int f,int s,char * out)
{
    for (int i=0;i<3;i++)
    out[i]=faces[f][i][s];
}
Kubick &Kubick::turn(int a)
{
    int choose;
    std::cout << "1- ->\n2- <-\n";
    std::cin >> choose;
    switch (choose)
    {
    case 1:
        char **arr = new char *[3];
        for (int i = 0; i < 3; i++)
            arr[i] = new char[3];

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                arr[j][2 - i] = faces[a][i][j];
            }
        }
        char xranenie[3];
        int list[4];
        Kubick::get_index(list,4,a);

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                faces[a][i][j] = arr[i][j];
            }
        }
        for(int i=0;i<3;i++)
        delete [] arr[i];
        delete []arr;
    }
}