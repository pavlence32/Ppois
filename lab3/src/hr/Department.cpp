#include "hr/Department.hpp"

#include "core/exceptions.hpp"

#include <algorithm>

Shift::Shift(int id, std::string name, int start_hour, int end_hour, int required_workers)
    : id_(id),
      name_(std::move(name)),
      start_hour_(start_hour),
      end_hour_(end_hour),
      required_workers_(required_workers)
{
    if (start_hour < 0 || start_hour > 23 || end_hour < 1 || end_hour > 24
        || start_hour >= end_hour)
        throw InvalidShiftException("Некорректные границы смены");
}

bool Shift::covers_hour(int hour) const
{
    return hour >= start_hour_ && hour < end_hour_;
}

int Shift::duration_hours() const
{
    return end_hour_ - start_hour_;
}

bool Shift::overlaps(const Shift& other) const
{
    return start_hour_ < other.end_hour_ && other.start_hour_ < end_hour_;
}

bool Shift::is_fully_staffed(int assigned) const
{
    return assigned >= required_workers_;
}

int Shift::missing_workers(int assigned) const
{
    return std::max(0, required_workers_ - assigned);
}

Department::Department(int id, std::string name, int manager_id, Money monthly_budget)
    : id_(id),
      name_(std::move(name)),
      manager_id_(manager_id),
      monthly_budget_(monthly_budget)
{
}

void Department::add_employee(int employee_id)
{
    if (employee_id <= 0)
        throw EntityNotFoundException("Сотрудник не найден");
    if (std::find(employee_ids_.begin(), employee_ids_.end(), employee_id) != employee_ids_.end())
        throw DuplicateEntityException("Сотрудник уже входит в подразделение");
    employee_ids_.push_back(employee_id);
}

bool Department::remove_employee(int employee_id)
{
    const auto it = std::find(employee_ids_.begin(), employee_ids_.end(), employee_id);
    if (it == employee_ids_.end())
        return false;
    employee_ids_.erase(it);
    return true;
}

Money Department::remaining_budget(const Money& spent) const
{
    if (!monthly_budget_.can_cover(spent))
        throw CapacityExceededException("Бюджет подразделения превышен");
    return monthly_budget_ - spent;
}

bool Department::has_manager() const
{
    return manager_id_ > 0;
}

void Department::replace_manager(int manager_id)
{
    if (manager_id <= 0)
        throw EntityNotFoundException("Руководитель не найден");
    manager_id_ = manager_id;
}

int Department::headcount() const
{
    return static_cast<int>(employee_ids_.size());
}

int Department::id() const
{
    return id_;
}
