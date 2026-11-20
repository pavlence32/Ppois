#pragma once

#include "core/Money.hpp"
#include "core/enums.hpp"
#include "hr/Person.hpp"

#include <string>
#include <vector>

class Employee : public Person
{
    int employee_id_;
    std::string position_;
    Money salary_;
    int hire_day_;
    int department_id_;
    int skill_level_;

public:
    Employee(int employee_id,
             std::string full_name,
             Address address,
             ContactInfo contact,
             std::string position,
             Money salary,
             int hire_day,
             int department_id = 0,
             int skill_level = 1);

    Money give_raise(double percent);
    Money compute_bonus(double rate) const;
    void assign_department(int department_id);
    bool is_eligible_for_promotion() const;
    void record_training(int points);
    Money annual_cost() const;
    EmployeeRole role() const;

    int employee_id() const;
    int department_id() const;
    const Money& salary() const;
    int skill_level() const;
    const std::string& position() const;
};

class Worker : public Employee
{
    std::string station_;
    int produced_units_;
    int shift_id_;

public:
    Worker(int employee_id,
           std::string full_name,
           Address address,
           ContactInfo contact,
           Money salary,
           int hire_day,
           int department_id,
           std::string station,
           int shift_id);

    void produce(int units);
    void reset_counter();
    void transfer_to(int shift_id, const std::string& station);
    bool meets_quota(int target) const;
};

class Foreman : public Employee
{
    int team_size_;
    std::vector<int> team_ids_;

public:
    Foreman(int employee_id,
            std::string full_name,
            Address address,
            ContactInfo contact,
            Money salary,
            int hire_day,
            int department_id,
            int team_size);

    void assign_worker(int worker_id);
    bool remove_worker(int worker_id);
    bool can_accept_more(int maximum) const;
    double evaluate_team_output(int produced, int target) const;
};

class Engineer : public Employee
{
    std::string specialty_;
    std::vector<int> machine_ids_;

public:
    Engineer(int employee_id,
             std::string full_name,
             Address address,
             ContactInfo contact,
             Money salary,
             int hire_day,
             int department_id,
             std::string specialty);

    void add_machine(int machine_id);
    bool can_service_machine(int machine_id) const;
    int service_load() const;
};

class Accountant : public Employee
{
    std::string ledger_;
    double tax_rate_;

public:
    Accountant(int employee_id,
               std::string full_name,
               Address address,
               ContactInfo contact,
               Money salary,
               int hire_day,
               int department_id,
               std::string ledger,
               double tax_rate);

    Money calculate_tax(const Money& gross) const;
    Money net_pay(const Money& gross) const;
    bool is_balanced(const Money& income, const Money& expenses) const;
};

class Storekeeper : public Employee
{
    int warehouse_id_;
    int handled_shipments_;

public:
    Storekeeper(int employee_id,
                std::string full_name,
                Address address,
                ContactInfo contact,
                Money salary,
                int hire_day,
                int department_id,
                int warehouse_id);

    void receive_shipment(int units);
    bool can_store(int units, int free_capacity) const;
    int processed_units() const;
};

class QualityInspector : public Employee
{
    std::string certificate_;
    int inspections_;

public:
    QualityInspector(int employee_id,
                     std::string full_name,
                     Address address,
                     ContactInfo contact,
                     Money salary,
                     int hire_day,
                     int department_id,
                     std::string certificate);

    void record_inspection();
    bool is_certified_for(const std::string& product_type) const;
    bool can_inspect_more(int daily_limit) const;
};

class SecurityGuard : public Employee
{
    std::string post_;
    std::vector<std::string> logged_incidents_;

public:
    SecurityGuard(int employee_id,
                  std::string full_name,
                  Address address,
                  ContactInfo contact,
                  Money salary,
                  int hire_day,
                  int department_id,
                  std::string post);

    void log_incident(const std::string& incident);
    bool has_open_incident() const;
    int incident_count() const;
};
