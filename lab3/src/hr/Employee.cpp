#include "hr/Employee.hpp"

#include "core/exceptions.hpp"

#include <algorithm>

Employee::Employee(int employee_id,
                   std::string full_name,
                   Address address,
                   ContactInfo contact,
                   std::string position,
                   Money salary,
                   int hire_day,
                   int department_id,
                   int skill_level)
    : Person(std::move(full_name), std::move(address), std::move(contact),
             "TAX-" + std::to_string(employee_id)),
      employee_id_(employee_id),
      position_(std::move(position)),
      salary_(salary),
      hire_day_(hire_day),
      department_id_(department_id),
      skill_level_(skill_level)
{
}

Money Employee::give_raise(double percent)
{
    if (percent <= 0.0)
        throw InvalidMoneyException("Процент повышения должен быть положительным");
    salary_ = salary_ + salary_.percent(percent);
    return salary_;
}

Money Employee::compute_bonus(double rate) const
{
    return salary_.percent(rate + skill_level_);
}

void Employee::assign_department(int department_id)
{
    if (department_id <= 0)
        throw EntityNotFoundException("Подразделение не найдено");
    department_id_ = department_id;
}

bool Employee::is_eligible_for_promotion() const
{
    return is_active() && skill_level_ >= 4;
}

void Employee::record_training(int points)
{
    if (points <= 0)
        return;
    skill_level_ = std::min(10, skill_level_ + points);
}

Money Employee::annual_cost() const
{
    return salary_ * 12.0 + compute_bonus(10.0);
}

EmployeeRole Employee::role() const
{
    if (position_ == "accountant")
        return EmployeeRole::Accountant;
    if (position_ == "engineer")
        return EmployeeRole::Engineer;
    if (position_ == "foreman")
        return EmployeeRole::Foreman;
    if (position_ == "storekeeper")
        return EmployeeRole::Storekeeper;
    if (position_ == "inspector")
        return EmployeeRole::QualityInspector;
    if (position_ == "guard")
        return EmployeeRole::SecurityGuard;
    return EmployeeRole::Worker;
}

int Employee::employee_id() const
{
    return employee_id_;
}

int Employee::department_id() const
{
    return department_id_;
}

const Money& Employee::salary() const
{
    return salary_;
}

int Employee::skill_level() const
{
    return skill_level_;
}

const std::string& Employee::position() const
{
    return position_;
}

Worker::Worker(int employee_id,
               std::string full_name,
               Address address,
               ContactInfo contact,
               Money salary,
               int hire_day,
               int department_id,
               std::string station,
               int shift_id)
    : Employee(employee_id, std::move(full_name), std::move(address), std::move(contact),
               "worker", salary, hire_day, department_id),
      station_(std::move(station)),
      produced_units_(0),
      shift_id_(shift_id)
{
}

void Worker::produce(int units)
{
    if (units <= 0)
        throw FactoryException("Количество изделий должно быть положительным");
    produced_units_ += units;
}

void Worker::reset_counter()
{
    produced_units_ = 0;
}

void Worker::transfer_to(int shift_id, const std::string& station)
{
    if (shift_id <= 0)
        throw InvalidShiftException("Некорректная смена");
    shift_id_ = shift_id;
    station_ = station;
}

bool Worker::meets_quota(int target) const
{
    return produced_units_ >= target;
}

Foreman::Foreman(int employee_id,
                 std::string full_name,
                 Address address,
                 ContactInfo contact,
                 Money salary,
                 int hire_day,
                 int department_id,
                 int team_size)
    : Employee(employee_id, std::move(full_name), std::move(address), std::move(contact),
               "foreman", salary, hire_day, department_id),
      team_size_(team_size)
{
}

void Foreman::assign_worker(int worker_id)
{
    if (static_cast<int>(team_ids_.size()) >= team_size_)
        throw CapacityExceededException("Бригада уже укомплектована");
    if (std::find(team_ids_.begin(), team_ids_.end(), worker_id) != team_ids_.end())
        throw DuplicateEntityException("Рабочий уже в бригаде");
    team_ids_.push_back(worker_id);
}

bool Foreman::remove_worker(int worker_id)
{
    const auto it = std::find(team_ids_.begin(), team_ids_.end(), worker_id);
    if (it == team_ids_.end())
        return false;
    team_ids_.erase(it);
    return true;
}

bool Foreman::can_accept_more(int maximum) const
{
    return static_cast<int>(team_ids_.size()) < maximum;
}

double Foreman::evaluate_team_output(int produced, int target) const
{
    if (target <= 0)
        return 0.0;
    return 100.0 * produced / target;
}

Engineer::Engineer(int employee_id,
                   std::string full_name,
                   Address address,
                   ContactInfo contact,
                   Money salary,
                   int hire_day,
                   int department_id,
                   std::string specialty)
    : Employee(employee_id, std::move(full_name), std::move(address), std::move(contact),
               "engineer", salary, hire_day, department_id),
      specialty_(std::move(specialty))
{
}

void Engineer::add_machine(int machine_id)
{
    if (std::find(machine_ids_.begin(), machine_ids_.end(), machine_id) != machine_ids_.end())
        throw DuplicateEntityException("Станок уже закреплён за инженером");
    machine_ids_.push_back(machine_id);
}

bool Engineer::can_service_machine(int machine_id) const
{
    return std::find(machine_ids_.begin(), machine_ids_.end(), machine_id) != machine_ids_.end();
}

int Engineer::service_load() const
{
    return static_cast<int>(machine_ids_.size());
}

Accountant::Accountant(int employee_id,
                       std::string full_name,
                       Address address,
                       ContactInfo contact,
                       Money salary,
                       int hire_day,
                       int department_id,
                       std::string ledger,
                       double tax_rate)
    : Employee(employee_id, std::move(full_name), std::move(address), std::move(contact),
               "accountant", salary, hire_day, department_id),
      ledger_(std::move(ledger)),
      tax_rate_(tax_rate)
{
}

Money Accountant::calculate_tax(const Money& gross) const
{
    return gross.percent(tax_rate_);
}

Money Accountant::net_pay(const Money& gross) const
{
    return gross - calculate_tax(gross);
}

bool Accountant::is_balanced(const Money& income, const Money& expenses) const
{
    return income.can_cover(expenses);
}

Storekeeper::Storekeeper(int employee_id,
                         std::string full_name,
                         Address address,
                         ContactInfo contact,
                         Money salary,
                         int hire_day,
                         int department_id,
                         int warehouse_id)
    : Employee(employee_id, std::move(full_name), std::move(address), std::move(contact),
               "storekeeper", salary, hire_day, department_id),
      warehouse_id_(warehouse_id),
      handled_shipments_(0)
{
}

void Storekeeper::receive_shipment(int units)
{
    if (units <= 0)
        throw FactoryException("Поставка должна содержать хотя бы одну единицу");
    handled_shipments_ += units;
}

bool Storekeeper::can_store(int units, int free_capacity) const
{
    return units > 0 && free_capacity >= units;
}

int Storekeeper::processed_units() const
{
    return handled_shipments_;
}

QualityInspector::QualityInspector(int employee_id,
                                   std::string full_name,
                                   Address address,
                                   ContactInfo contact,
                                   Money salary,
                                   int hire_day,
                                   int department_id,
                                   std::string certificate)
    : Employee(employee_id, std::move(full_name), std::move(address), std::move(contact),
               "inspector", salary, hire_day, department_id),
      certificate_(std::move(certificate)),
      inspections_(0)
{
}

void QualityInspector::record_inspection()
{
    ++inspections_;
}

bool QualityInspector::is_certified_for(const std::string& product_type) const
{
    return certificate_.find(product_type) != std::string::npos;
}

bool QualityInspector::can_inspect_more(int daily_limit) const
{
    return inspections_ < daily_limit;
}

SecurityGuard::SecurityGuard(int employee_id,
                             std::string full_name,
                             Address address,
                             ContactInfo contact,
                             Money salary,
                             int hire_day,
                             int department_id,
                             std::string post)
    : Employee(employee_id, std::move(full_name), std::move(address), std::move(contact),
               "guard", salary, hire_day, department_id),
      post_(std::move(post))
{
}

void SecurityGuard::log_incident(const std::string& incident)
{
    if (incident.empty())
        throw SafetyViolationException("Инцидент должен быть описан");
    logged_incidents_.push_back(incident);
}

bool SecurityGuard::has_open_incident() const
{
    return !logged_incidents_.empty();
}

int SecurityGuard::incident_count() const
{
    return static_cast<int>(logged_incidents_.size());
}
