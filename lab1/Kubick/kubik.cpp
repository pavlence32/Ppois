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
void Kubick::set_row(int f,int s,char * out,bool rev)
{
    for (int i=0;i<3;i++)
    faces[f][s][i]=out[rev ? 2-i : i];
}
void Kubick::get_col(int f,int s,char * out)
{
    for (int i=0;i<3;i++)
    out[i]=faces[f][i][s];
}
void Kubick::set_col(int f,int s,char * out,bool rev)
{
    for (int i=0;i<3;i++)
    faces[f][i][s]=out[rev ? 2-i : i];
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
void Kubick::shift_faces0()
{
    char saved[3], strip[3];
    get_row(2, 0, saved);
    get_row(5, 0, strip); set_row(2, 0, strip);
    get_row(4, 0, strip); set_row(5, 0, strip);
    get_row(3, 0, strip); set_row(4, 0, strip);
    set_row(3, 0, saved);
}
void Kubick::shift_faces1()
{
    char saved[3], strip[3];
    get_row(2, 2, saved);
    get_row(3, 2, strip); set_row(2, 2, strip);
    get_row(4, 2, strip); set_row(3, 2, strip);
    get_row(5, 2, strip); set_row(4, 2, strip);
    set_row(5, 2, saved);
}
void Kubick::shift_faces2()
{
    char saved[3], strip[3];
    get_row(0, 2, saved);
    get_col(5, 2, strip); set_row(0, 2, strip,true);
    get_row(1, 0, strip); set_col(5, 2, strip);
    get_col(3, 0, strip); set_row(1, 0, strip,true);
    set_col(3, 0, saved);
}
void Kubick::shift_faces3()
{
    char saved[3], strip[3];
    get_col(0, 2, saved);
    get_col(2, 2, strip); set_col(0, 2, strip);
    get_col(1, 2, strip); set_col(2, 2, strip);
    get_col(4, 0, strip); set_col(1, 2, strip,true);
    set_col(4, 0, saved,true);
}
void Kubick::shift_faces4()
{
    char saved[3], strip[3];
    get_row(0, 0, saved);
    get_col(3, 2, strip); set_row(0, 0, strip);
    get_row(1, 2, strip); set_col(3, 2, strip,true);
    get_col(5, 0, strip); set_row(1, 2, strip);
    set_col(5, 0, saved,true);
}
void Kubick::shift_faces5()
{
    char saved[3], strip[3];
    get_col(0, 0, saved);
    get_col(4, 2, strip); set_col(0, 0, strip,true);
    get_col(1, 0, strip); set_col(4, 2, strip,true);
    get_col(2, 0, strip); set_col(1, 0, strip);
    set_col(2, 0, saved);
}
void Kubick::shift_neighbours(int face)
{
    switch (face)
    {
        case 0: Kubick::shift_faces0();  
        break;
        case 1: Kubick::shift_faces1();    
        break;
        case 2: Kubick::shift_faces2();  
        break;
        case 3: Kubick::shift_faces3(); 
        break;
        case 4: Kubick::shift_faces4();  
        break;
        case 5:Kubick::shift_faces5(); 
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