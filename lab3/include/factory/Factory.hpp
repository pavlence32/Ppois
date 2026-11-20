#pragma once

#include "core/Address.hpp"
#include "core/Money.hpp"
#include "hr/Department.hpp"
#include "hr/Employee.hpp"
#include "production/ProductionLine.hpp"
#include "production/Warehouse.hpp"
#include "production/WorkOrder.hpp"

#include <string>
#include <vector>

class Factory
{
    std::string name_;
    Address address_;
    std::vector<Department> departments_;
    std::vector<ProductionLine> lines_;
    Warehouse warehouse_;
    Money operating_budget_;
    std::vector<Employee> employees_;

public:
    Factory(std::string name, Address address, Warehouse warehouse, Money operating_budget);

    void add_department(const Department& department);
    void hire(const Employee& employee);
    bool can_produce(const WorkOrder& order) const;
    Money total_payroll() const;
    double overall_efficiency() const;
    void schedule(WorkOrder& order, ProductionLine& line);
    Department* find_department(int id);
    int employee_count() const;
};
