#pragma once

#include "model.hpp"

#include <string>

class Room;
class Guest;

class RoomKey
{
    std::string id_;
    Room* room_;
    Guest* owner_;
    Date valid_from_;
    Date valid_until_;
    bool active_;
    std::string access_code_;
    int use_count_ = 0;
    bool copied_ = false;

public:
    RoomKey(std::string id, Room* room, Guest* owner, Date valid_from, Date valid_until);
    bool is_valid(Date now) const;
    void open(Room* room);
    void extend(Date new_until);
    void deactivate();
    bool belongs_to(Guest* guest) const;
    bool opens_room(Room* room) const;
    int remaining_days(Date now) const;
};
