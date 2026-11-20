#pragma once

#include "core/Money.hpp"

#include <string>
#include <vector>

class Shift
{
    int id_;
    std::string name_;
    int start_hour_;
    int end_hour_;
    int required_workers_;

public:
    Shift(int id, std::string name, int start_hour, int end_hour, int required_workers);

    bool covers_hour(int hour) const;
    int duration_hours() const;
    bool overlaps(const Shift& other) const;
    bool is_fully_staffed(int assigned) const;
    int missing_workers(int assigned) const;
};

class Department
{
    int id_;
    std::string name_;
    int manager_id_;
    std::vector<int> employee_ids_;
    Money monthly_budget_;

public:
    Department(int id, std::string name, int manager_id, Money monthly_budget);

    void add_employee(int employee_id);
    bool remove_employee(int employee_id);
    Money remaining_budget(const Money& spent) const;
    bool has_manager() const;
    void replace_manager(int manager_id);
    int headcount() const;
    int id() const;
};
