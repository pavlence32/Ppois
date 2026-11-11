#pragma once

#include "people/employee.hpp"
#include "core/model.hpp"

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
    std::string phone_;
    std::string email_;
    std::string website_;
    double tax_rate_ = 0.0;
    bool open_ = true;
    int parking_places_ = 0;
    std::string manager_name_;
    std::string timezone_;
    bool pets_allowed_ = false;
    bool conference_hall_ = false;
    int total_floors_ = 0;
    std::string emergency_phone_;
    std::string breakfast_time_;
    std::string restaurant_name_;
    std::string currency_;
    std::string registration_number_;

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
    std::size_t room_count() const;
    std::size_t employee_count() const;
    bool has_available_room(Date start, Date end, int guests) const;
    void clean_all_dirty_rooms();
};
