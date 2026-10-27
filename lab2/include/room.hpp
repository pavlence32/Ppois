#include <string>
#include <vector>
class Room
{
int roomkey;
bool status;
int flot;
public:
Room(int roomkey,bool status,int flot);
int get_roomkey();
int get_status();
void set_status();
};