#pragma once
#include "employee.hpp"
#include "receptionist.hpp"
#include "chef.hpp"
#include "housekeeper.hpp"
#include "exception.hpp"
#include <vector>
class HotelManager
{
private:
    std::vector<Chef> chefs;
    std::vector<Housekeeper> housekeepers;
    std::vector<receptionist> receptionists;

public:
    void addChef(const Chef& chef);
    void addHousekeeper(const Housekeeper& housekeeper);
    void addReceptionist(const receptionist& receptionist);

    void assignChef();
    void assignHousekeeper(int roomNumber);
    void assignReceptionist();
};