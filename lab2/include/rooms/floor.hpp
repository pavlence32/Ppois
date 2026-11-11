#pragma once

#include "core/model.hpp"

#include <vector>

class Hotel;
class Room;

class Floor
{
    int number_;
    std::vector<Room*> rooms_;
    Hotel* hotel_;
    std::string name_;
    bool fire_exit_ = false;
    bool under_repair_ = false;
    int max_people_ = 0;

public:
    Floor(int number, Hotel* hotel = nullptr);
    int get_number() const;
    void add_room(Room* room);
    std::vector<Room*> find_free_rooms(Date start, Date end) const;
    std::vector<Room*> get_rooms_to_clean() const;
    std::size_t count_occupied() const;
    const std::vector<Room*>& get_rooms() const;
    std::size_t count_free() const;
    std::size_t count_dirty() const;
    Room* find_room(int number) const;
};
