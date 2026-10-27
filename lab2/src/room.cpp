#include "room.hpp"
Room::Room(int roomkey,bool status,int flot):roomkey{roomkey},status{status},flot{flot}
{
}
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
