#pragma once

#include "core/Money.hpp"
#include "core/enums.hpp"

#include <string>

class Machine
{
    int id_;
    std::string name_;
    MachineStatus status_;
    double efficiency_;
    int operating_hours_;
    int maintenance_interval_hours_;
    int line_id_;

public:
    Machine(int id,
            std::string name,
            double efficiency,
            int maintenance_interval_hours,
            int line_id = 0);

    void start();
    void stop();
    void record_operation(int hours);
    bool needs_maintenance() const;
    void complete_maintenance();
    double availability() const;
    void report_breakdown();
    MachineStatus status() const;
    int id() const;
};

class MaintenanceRecord
{
    int machine_id_;
    int day_;
    std::string technician_;
    std::string description_;
    Money cost_;
    bool completed_;

public:
    MaintenanceRecord(int machine_id,
                      int day,
                      std::string technician,
                      std::string description,
                      Money cost);

    void complete();
    bool is_overdue(int current_day, int limit_days) const;
    Money cost_with_parts(const Money& parts) const;
    bool is_completed() const;
    int machine_id() const;
};
