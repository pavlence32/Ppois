#define kubik.h
#include <string>
class Kubick
{
    char faces[6][3][3];
    char fill_place(int index);
    void get_index(int * arr,int size,int a);
    void Kubick::get_row(int f,int s,char * out);
    void Kubick::set_row(int f,int s,char * out);
    void Kubick::get_col(int f,int s,char * out);
    void Kubick::set_col(int f,int s,char * out);
    public:
    Kubick();
    void load_color(std::string s);
    Kubick &turn(int a);
    bool checkcolorplace();
    void random_place();
};