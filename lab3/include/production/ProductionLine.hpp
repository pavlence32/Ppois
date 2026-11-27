#pragma once

#include <string>
#include <vector>

class ProductionLine
{
    int id_;
    std::string name_;
    std::vector<int> machine_ids_;
    double target_rate_;
    bool active_;

public:
    ProductionLine(int id, std::string name, double target_rate, bool active = false);

    void add_machine(int machine_id);
    bool remove_machine(int machine_id);
    bool can_run() const;
    double capacity_per_hour() const;
    void activate();
    void deactivate();
    bool is_active() const;
};
