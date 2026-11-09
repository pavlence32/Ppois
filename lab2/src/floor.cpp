#include "floor.hpp"

#include "exception.hpp"
#include "room.hpp"

Floor::Floor(int number, Hotel* hotel)
    : number_(number), hotel_(hotel)
{}

int Floor::get_number() const
{
    return number_;
}

void Floor::add_room(Room* room)
{
    if (room == nullptr)
        throw DuplicateRoomException{"Нельзя добавить пустой номер"};
    for (Room* current : rooms_)
    {
        if (current->get_number() == room->get_number())
            throw DuplicateRoomException{"Номер с таким номером уже существует"};
    }
    rooms_.push_back(room);
}

std::vector<Room*> Floor::find_free_rooms(Date start, Date end) const
{
    std::vector<Room*> result;
    for (Room* room : rooms_)
    {
        if (room->is_available(start, end))
            result.push_back(room);
    }
    return result;
}

std::vector<Room*> Floor::get_rooms_to_clean() const
{
    std::vector<Room*> result;
    for (Room* room : rooms_)
    {
        if (room->get_status() == RoomStatus::Dirty)
            result.push_back(room);
    }
    return result;
}

std::size_t Floor::count_occupied() const
{
    std::size_t count = 0;
    for (Room* room : rooms_)
    {
        if (room->get_status() == RoomStatus::Occupied)
            count++;
    }

    return count;
}

std::size_t Floor::count_free() const
{
    std::size_t count = 0;
    for (Room* room : rooms_)
        if (room->get_status() == RoomStatus::Free)
            ++count;
    return count;
}

std::size_t Floor::count_dirty() const
{
    std::size_t count = 0;
    for (Room* room : rooms_)
        if (room->get_status() == RoomStatus::Dirty)
            ++count;
    return count;
}

Room* Floor::find_room(int number) const
{
    for (Room* room : rooms_)
        if (room->get_number() == number)
            return room;
    return nullptr;
}

const std::vector<Room*>& Floor::get_rooms() const
{
    return rooms_;
}
