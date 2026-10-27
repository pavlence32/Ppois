#include "room.hpp"
class StandartRoom:public Room
{
    std::vector<std::string> amenity;
    public:
    StandartRoom(int roomkey,bool status,int flot,std::vector<std::string>amenity);


};