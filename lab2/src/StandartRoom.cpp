#include "StandartRoom.hpp"
StandartRoom::StandartRoom(int roomkey,bool status,int flot,std::vector<std::string> amenity):Room::Room(roomkey,status,flot),amenity{amenity}
{
};