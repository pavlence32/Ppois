#include <string>
#include <vector>
#include "Roomtype.hpp"
#include "Bed.hpp"
class Room
{
int number;
RoomType type;
float base_price;
int roomkey;
bool status;
int floor;
Bed bed;
public:
Room(int number,std::string type,float base_price,int roomkey,bool status,int floor,Bed bed,float priceMultiplier);
int get_roomkey();
int get_status();
void set_status();
void get_price();
};