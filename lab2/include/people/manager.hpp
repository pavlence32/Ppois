#pragma once
#include "people/employee.hpp"
#include "people/receptionist.hpp"
#include "people/chef.hpp"
#include "people/housekeeper.hpp"
#include "core/exception.hpp"
#include <vector>
class HotelManager
{
private:
    std::vector<Chef> chefs;
    std::vector<Housekeeper> housekeepers;
    std::vector<receptionist> receptionists;
    std::string office_;
    bool on_duty_ = false;
    int managed_rooms_ = 0;

public:
    void addChef(const Chef& chef);
    void addHousekeeper(const Housekeeper& housekeeper);
    void addReceptionist(const receptionist& receptionist);

    void assignChef();
    void assignHousekeeper(int roomNumber);
    void assignReceptionist();
};