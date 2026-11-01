#pragma once

#include "employee.hpp"
#include "model.hpp"

#include <string>
#include <vector>

class Floor;
class Room;

class Hotel
{
    std::string name_;
    int stars_;
    Address address_;
    std::vector<Floor*> floors_;
    std::vector<Employee*> employees_;
    std::string check_in_time_;
    std::string check_out_time_;

public:
    Hotel(std::string name, int stars, Address address,
          std::string check_in_time = "14:00",
          std::string check_out_time = "12:00");
    void add_floor(Floor* floor);
    Room& find_room(int number) const;
    std::vector<Room*> find_available_rooms(Date start, Date end, int guests) const;
    double get_occupancy_rate() const;
    void hire_employee(Employee* employee);
    const std::string& get_check_out_time() const;
};
