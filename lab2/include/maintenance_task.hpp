#pragma once

#include <string>

class MaintenanceTask
{
    int room_number_;
    std::string description_;
    bool completed_;

public:
    MaintenanceTask(int room_number, std::string description);
    void complete();
    bool is_completed() const;
    bool concerns_room(int number) const;
};
