#include "hotel.hpp"

#include "employee.hpp"
#include "exception.hpp"
#include "floor.hpp"
#include "room.hpp"

Hotel::Hotel(std::string name, int stars, Address address,
             std::string check_in_time, std::string check_out_time)
    : name_(name),
      stars_(stars),
      address_(address),
      check_in_time_(check_in_time),
      check_out_time_(check_out_time)
{}

void Hotel::add_floor(Floor* floor)
{
    if (floor != nullptr)
        floors_.push_back(floor);
}

Room& Hotel::find_room(int number) const
{
    for (Floor* floor : floors_)
    {
        for (Room* room : floor->get_rooms())
        {
            if (room->get_number() == number)
                return *room;
        }
    }
    throw RoomNotFoundException{"Номер не найден"};
}

std::vector<Room*> Hotel::find_available_rooms(Date start, Date end, int guests) const
{
    std::vector<Room*> result;
    for (Floor* floor : floors_)
    {
        for (Room* room : floor->find_free_rooms(start, end))
        {
            try
            {
                room->can_accommodate(guests);
                result.push_back(room);
            }
            catch (const RoomOverCapacityException&)
            {
            }
        }
    }
    return result;
}

double Hotel::get_occupancy_rate() const
{
    std::size_t total = 0;
    std::size_t occupied = 0;
    for (Floor* floor : floors_)
    {
        total += floor->get_rooms().size();
        occupied += floor->count_occupied();
    }
    if (total == 0)
        return 0.0;
    return 100.0 * static_cast<double>(occupied) / total;
}

void Hotel::hire_employee(Employee* employee)
{
    if (employee != nullptr)
        employees_.push_back(employee);
}

const std::string& Hotel::get_check_out_time() const
{
    return check_out_time_;
}

std::size_t Hotel::room_count() const
{
    std::size_t count = 0;
    for (Floor* floor : floors_)
        count += floor->get_rooms().size();
    return count;
}

std::size_t Hotel::employee_count() const
{
    return employees_.size();
}

bool Hotel::has_available_room(Date start, Date end, int guests) const
{
    return !find_available_rooms(start, end, guests).empty();
}

void Hotel::clean_all_dirty_rooms()
{
    for (Floor* floor : floors_)
        for (Room* room : floor->get_rooms())
            if (room->get_status() == RoomStatus::Dirty)
                room->mark_clean();
}
