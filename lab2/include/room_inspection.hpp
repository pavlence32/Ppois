#pragma once
#include <string>
class RoomInspection
{
    int room_number_;
    std::string inspector_;
    bool passed_;
public:
    RoomInspection(int room_number, std::string inspector);
    void pass();
    bool passed() const;
    bool concerns_room(int room_number) const;
};
