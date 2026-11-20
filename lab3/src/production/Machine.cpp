#include "production/Machine.hpp"

#include "core/exceptions.hpp"

#include <algorithm>

Machine::Machine(int id,
                 std::string name,
                 double efficiency,
                 int maintenance_interval_hours,
                 int line_id)
    : id_(id),
      name_(std::move(name)),
      status_(MachineStatus::Idle),
      efficiency_(efficiency),
      operating_hours_(0),
      maintenance_interval_hours_(maintenance_interval_hours),
      line_id_(line_id)
{
    if (efficiency < 0.0 || efficiency > 1.0)
        throw FactoryException("КПД станка должен быть в диапазоне 0..1");
}

void Machine::start()
{
    if (status_ == MachineStatus::Broken || status_ == MachineStatus::UnderMaintenance)
        throw MachineUnavailableException("Станок " + name_ + " недоступен");
    status_ = MachineStatus::Running;
}

void Machine::stop()
{
    if (status_ == MachineStatus::Running)
        status_ = MachineStatus::Idle;
}

void Machine::record_operation(int hours)
{
    if (hours <= 0)
        throw FactoryException("Наработка должна быть положительной");
    if (status_ != MachineStatus::Running)
        throw MachineUnavailableException("Нельзя нарабатывать часы на остановленном станке");
    operating_hours_ += hours;
    if (needs_maintenance())
        status_ = MachineStatus::UnderMaintenance;
}

bool Machine::needs_maintenance() const
{
    return operating_hours_ >= maintenance_interval_hours_;
}

void Machine::complete_maintenance()
{
    operating_hours_ = 0;
    if (status_ != MachineStatus::Broken)
        status_ = MachineStatus::Idle;
}

double Machine::availability() const
{
    if (status_ == MachineStatus::Broken)
        return 0.0;
    if (maintenance_interval_hours_ <= 0)
        return efficiency_;
    const double wear = static_cast<double>(operating_hours_) / maintenance_interval_hours_;
    return efficiency_ * std::max(0.0, 1.0 - wear * 0.2);
}

void Machine::report_breakdown()
{
    status_ = MachineStatus::Broken;
}

MachineStatus Machine::status() const
{
    return status_;
}

int Machine::id() const
{
    return id_;
}

MaintenanceRecord::MaintenanceRecord(int machine_id,
                                     int day,
                                     std::string technician,
                                     std::string description,
                                     Money cost)
    : machine_id_(machine_id),
      day_(day),
      technician_(std::move(technician)),
      description_(std::move(description)),
      cost_(cost),
      completed_(false)
{
}

void MaintenanceRecord::complete()
{
    if (description_.empty())
        throw FactoryException("Нельзя закрыть работы без описания");
    completed_ = true;
}

bool MaintenanceRecord::is_overdue(int current_day, int limit_days) const
{
    return !completed_ && current_day - day_ > limit_days;
}

Money MaintenanceRecord::cost_with_parts(const Money& parts) const
{
    return cost_ + parts;
}

bool MaintenanceRecord::is_completed() const
{
    return completed_;
}

int MaintenanceRecord::machine_id() const
{
    return machine_id_;
}
