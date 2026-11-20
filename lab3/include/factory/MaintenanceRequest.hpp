#pragma once

#include "core/Money.hpp"
#include "core/enums.hpp"

#include <string>

class MaintenanceRequest
{
    int id_;
    int machine_id_;
    std::string description_;
    Priority priority_;
    bool completed_;
    Money estimated_cost_;

public:
    MaintenanceRequest(int id,
                       int machine_id,
                       std::string description,
                       Priority priority,
                       Money estimated_cost);

    void escalate();
    Money complete(const Money& actual_cost);
    bool is_urgent() const;
    int priority_rank() const;
    bool is_completed() const;
    int machine_id() const;
};
