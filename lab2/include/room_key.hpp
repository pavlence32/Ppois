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

public:
    RoomKey(std::string id, Room* room, Guest* owner, Date valid_from, Date valid_until);
    bool is_valid(Date now) const;
    void open(Room* room);
    void extend(Date new_until);
    void deactivate();
};
