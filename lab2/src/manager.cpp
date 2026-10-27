#include "manager.hpp"
#include <iostream>
void HotelManager::addChef(const Chef &chef)
{
    chefs.push_back(chef);
    std::cout << chef.getName() << " был нанят" << std::endl;
}
void HotelManager::addHousekeeper(const Housekeeper &housekeeper)
{
    housekeepers.push_back(housekeeper);
    std::cout << housekeeper.getName() << " был нанят" << std::endl;
}
void HotelManager::addReceptionist(const receptionist &receptionist)
{
    receptionists.push_back(receptionist);
    std::cout << receptionist.getName() << " был нанят" << std::endl;
}
void HotelManager::assignChef()
{
    for (auto &chef : chefs)
    {
        if (!chef.get_status())
        {
            chef.set_status(true);
            std::cout << chef.getName() << " назначен на работу" << std::endl;
            return ;
        }
    }
    throw EmployeeUnavailableException{"Нет доступных поваров"};
}
void HotelManager::assignHousekeeper(int roomNumber)
{
    for (auto &housekeeper : housekeepers)
    {
        if (!housekeeper.get_status())                  
        {
            housekeeper.set_status(true);
            std::cout << housekeeper.getName() << " назначен на уборку комнаты " << roomNumber << std::endl;
            return ;
        }
    }
    throw EmployeeUnavailableException{"Нет доступных уборщиков"};
}
void HotelManager::assignReceptionist()
{
    for (auto &receptionist : receptionists)
    {
        if (!receptionist.get_status())             
        {
            receptionist.set_status(true);
            std::cout << receptionist.getName() << " назначен на работу" << std::endl;
            return;
        }                   
    }
    throw EmployeeUnavailableException{"Нет доступных ресепшн-менеджеров"};
}