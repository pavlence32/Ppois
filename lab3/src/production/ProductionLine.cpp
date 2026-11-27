#include "production/ProductionLine.hpp"

#include "core/exceptions.hpp"

#include <algorithm>

ProductionLine::ProductionLine(int id, std::string name, double target_rate, bool active)
    : id_(id), name_(std::move(name)), target_rate_(target_rate), active_(active)
{
    if (target_rate <= 0.0)
        throw FactoryException("Целевая производительность должна быть положительной");
}

void ProductionLine::add_machine(int machine_id)
{
    if (std::find(machine_ids_.begin(), machine_ids_.end(), machine_id) != machine_ids_.end())
        throw DuplicateEntityException("Станок уже установлен на линии");
    machine_ids_.push_back(machine_id);
}

bool ProductionLine::remove_machine(int machine_id)
{
    const auto it = std::find(machine_ids_.begin(), machine_ids_.end(), machine_id);
    if (it == machine_ids_.end())
        return false;
    machine_ids_.erase(it);
    if (!can_run())
        active_ = false;
    return true;
}

bool ProductionLine::can_run() const
{
    return machine_ids_.size() >= 2;
}

double ProductionLine::capacity_per_hour() const
{
    return target_rate_ * static_cast<double>(machine_ids_.size()) / 2.0;
}

void ProductionLine::activate()
{
    if (!can_run())
        throw MachineUnavailableException("Для запуска линии нужно минимум два станка");
    active_ = true;
}

void ProductionLine::deactivate()
{
    active_ = false;
}

bool ProductionLine::is_active() const
{
    return active_;
}
