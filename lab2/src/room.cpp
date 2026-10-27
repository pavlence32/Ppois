#include "room.hpp"
Room::Room(int number,std::string type,float base_price,int roomkey,bool status,int floor,Bed bed,float priceMultiplier)
    : number(number),
      type(type, priceMultiplier),
      base_price(base_price),
      roomkey(roomkey),
      status(status),
      floor(floor),
      bed(bed)
{}
int Room::get_roomkey()
{
    return roomkey;
}
int Room::get_status()
{
    return status;
}
void Room::set_status()
{
    status=!status;
}                   
