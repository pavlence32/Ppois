#include "factory/Factory.hpp"

#include "core/exceptions.hpp"

#include <algorithm>

Factory::Factory(std::string name, Address address, Warehouse warehouse, Money operating_budget)
    : name_(std::move(name)),
      address_(std::move(address)),
      warehouse_(std::move(warehouse)),
      operating_budget_(operating_budget)
{
    if (!operating_budget_.is_positive())
        throw InvalidMoneyException("Бюджет завода должен быть положительным");
}

void Factory::add_department(const Department& department)
{
    if (find_department(department.id()) != nullptr)
        throw DuplicateEntityException("Подразделение с таким номером уже существует");
    departments_.push_back(department);
}

void Factory::hire(const Employee& employee)
{
    for (const auto& existing : employees_)
    {
        if (existing.employee_id() == employee.employee_id())
            throw DuplicateEntityException("Сотрудник уже работает на заводе");
    }
    employees_.push_back(employee);
}

bool Factory::can_produce(const WorkOrder& order) const
{
    if (order.status() == OrderStatus::Cancelled || order.status() == OrderStatus::Completed)
        return false;
    if (order.remaining_quantity() <= 0)
        return false;
    return operating_budget_.is_positive() && !lines_.empty();
}

Money Factory::total_payroll() const
{
    Money result(0.0);
    for (const auto& employee : employees_)
        result += employee.salary();
    return result;
}

double Factory::overall_efficiency() const
{
    if (employees_.empty())
        return 0.0;
    int total = 0;
    for (const auto& employee : employees_)
        total += employee.skill_level();
    return static_cast<double>(total) / (employees_.size() * 10.0);
}

void Factory::schedule(WorkOrder& order, ProductionLine& line)
{
    if (!can_produce(order))
        throw InvalidOrderStateException("Заказ нельзя поставить в производство");
    line.activate();
    if (order.status() == OrderStatus::Draft)
        order.confirm();
}

Department* Factory::find_department(int id)
{
    const auto it = std::find_if(departments_.begin(), departments_.end(),
                                 [id](const Department& department) {
                                     return department.id() == id;
                                 });
    return it == departments_.end() ? nullptr : &*it;
}

int Factory::employee_count() const
{
    return static_cast<int>(employees_.size());
}
