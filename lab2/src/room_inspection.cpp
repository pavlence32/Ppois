#include "room_inspection.hpp"
RoomInspection::RoomInspection(int room_number, std::string inspector)
    : room_number_(room_number), inspector_(inspector), passed_(false) {}
void RoomInspection::pass() { passed_ = true; }
bool RoomInspection::passed() const { return passed_; }
bool RoomInspection::concerns_room(int room_number) const
{
    return room_number_ == room_number;
}
