#include "kubik.h"
#include <ctime>
#include <cstdlib>
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
    default:
        return '?';
    }
}
bool operator==(const Kubick &b,const Kubick&a)
{
    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 3; j++)
            for (int k = 0; k < 3; k++)
                if (b.faces[i][j][k] != a.faces[i][j][k])
                    return false;
    return true;
}
bool operator!=(const Kubick&b,const Kubick &a)
{
    return !(b==a);
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
bool Kubick::load_color(std::istream& inf)
{
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                char c;
                if (!(inf >> c))
                {return false;}
                faces[i][j][k] = c;
            }
        }
    }
    return true;
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
    Kubick a;
    return *this==a;
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
    faces[f][i][s]=out[i];
}
void Kubick::rotate_face_matrix(int face)
{
    char faceh[3][3];
    for (int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            faceh[j][2-i]=faces[face][i][j];
        }
    }
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            faces[face][i][j]=faceh[i][j];
        }
    }
}
void Kubick::shift_neighbours(int face)
{
    char temp[3], buf[3];

    switch (face)
    {
        case 0: 
            get_row(2, 0, temp);
            get_row(5, 0, buf); set_row(2, 0, buf);
            get_row(4, 0, buf); set_row(5, 0, buf);
            get_row(3, 0, buf); set_row(4, 0, buf);
            set_row(3, 0, temp);
            break;

        case 1: 
            get_row(2, 2, temp);
            get_row(3, 2, buf); set_row(2, 2, buf);
            get_row(4, 2, buf); set_row(3, 2, buf);
            get_row(5, 2, buf); set_row(4, 2, buf);
            set_row(5, 2, temp);
            break;

        case 2: 
            get_row(0, 2, temp);
            get_col(5, 2, buf); set_row(0, 2, buf);
            get_row(1, 0, buf); set_col(5, 2, buf);
            get_col(3, 0, buf); set_row(1, 0, buf);
            set_col(3, 0, temp);
            break;

        case 3: 
            get_col(0, 2, temp);
            get_col(2, 2, buf); set_col(0, 2, buf);
            get_col(1, 2, buf); set_col(2, 2, buf);
            get_col(4, 0, buf); set_col(1, 2, buf);
            set_col(4, 0, temp);
            break;

        case 4: 
            get_row(0, 0, temp);
            get_col(3, 2, buf); set_row(0, 0, buf);
            get_row(1, 2, buf); set_col(3, 2, buf);
            get_col(5, 0, buf); set_row(1, 2, buf);
            set_col(5, 0, temp);
            break;

        case 5:
            get_col(0, 0, temp);
            get_col(4, 2, buf); set_col(0, 0, buf);
            get_col(1, 0, buf); set_col(4, 2, buf);
            get_col(2, 0, buf); set_col(1, 0, buf);
            set_col(2, 0, temp);
            break;
    }
}
std::ostream& operator<<(std::ostream &out,Kubick &a)
{
    for (int i = 0; i < 6; i++)
    {
        out << i << ": ";
        for (int j = 0; j < 3; j++)
        {
            for (int m = 0; m < 3; m++)
            {
                out << a.faces[i][j][m];
                if (m != 2)
                    out << ' ';
            }
            out << '\n';
        }
    }
    return out;

}
Kubick &Kubick::turn(int a,int trav)
{
    switch(trav)
    {
        case 1:
        rotate_face_matrix(a);
        shift_neighbours(a);
        return *this;
        case 0:
        for (int i=0;i<3;i++)
        {rotate_face_matrix(a);
        shift_neighbours(a);}
        return * this;
    }

}