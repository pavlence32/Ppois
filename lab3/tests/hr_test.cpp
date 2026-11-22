#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "core/Address.hpp"
#include "core/ContactInfo.hpp"
#include "core/Money.hpp"
#include "core/enums.hpp"
#include "core/exceptions.hpp"
#include "hr/AttendanceRecord.hpp"
#include "hr/Department.hpp"
#include "hr/Employee.hpp"
#include "hr/Person.hpp"

namespace
{
Address factory_address()
{
    return Address("Main street", "Minsk", "220000", "Belarus", "12");
}

ContactInfo factory_contact()
{
    return ContactInfo("+375291234567", "worker@factory.by", "office 3");
}
} // namespace

TEST_CASE("Person maintains a profile and contact data")
{
    Person person("John Doe", factory_address(), factory_contact(), "TAX-1");

    REQUIRE(person.full_name() == "John Doe");
    REQUIRE(person.initials() == "JD");
    REQUIRE(person.is_active());
    REQUIRE(person.has_complete_profile());
    REQUIRE(person.contact().email() == "worker@factory.by");

    ContactInfo valid("+375291112233", "new@factory.by", "office 5");
    person.update_contact(valid);
    REQUIRE(person.contact().email() == "new@factory.by");
    REQUIRE_THROWS_AS(person.update_contact(ContactInfo("+375291112233", "broken", "")),
                      UnauthorizedAccessException);

    person.relocate(Address("Second street", "Minsk", "220001", "Belarus", "7"));
    REQUIRE(person.address().city() == "Minsk");
    REQUIRE_THROWS_AS(person.relocate(Address("", "Minsk", "", "Belarus", "")), FactoryException);

    person.deactivate();
    REQUIRE_FALSE(person.is_active());

    Person incomplete("", Address("", "", "", "", ""), ContactInfo(), "");
    REQUIRE_FALSE(incomplete.has_complete_profile());
}

TEST_CASE("Employee calculates salary, bonus and role")
{
    Employee employee(1, "Alice Smith", factory_address(), factory_contact(),
                      "engineer", Money(1000.0), 10, 2, 3);

    REQUIRE(employee.employee_id() == 1);
    REQUIRE(employee.department_id() == 2);
    REQUIRE(employee.skill_level() == 3);
    REQUIRE(employee.position() == "engineer");
    REQUIRE(employee.salary().value() == 1000.0);
    REQUIRE(employee.role() == EmployeeRole::Engineer);

    REQUIRE(employee.compute_bonus(10.0).value() == Catch::Approx(130.0));
    REQUIRE(employee.give_raise(10.0).value() == Catch::Approx(1100.0));
    REQUIRE(employee.salary().value() == Catch::Approx(1100.0));
    REQUIRE_THROWS_AS(employee.give_raise(0.0), InvalidMoneyException);

    REQUIRE(employee.annual_cost().value() == Catch::Approx(1100.0 * 12.0 + 1100.0 * 0.13));

    employee.record_training(20);
    REQUIRE(employee.skill_level() == 10);
    employee.record_training(0);
    REQUIRE(employee.skill_level() == 10);
    REQUIRE(employee.is_eligible_for_promotion());

    employee.assign_department(4);
    REQUIRE(employee.department_id() == 4);
    REQUIRE_THROWS_AS(employee.assign_department(0), EntityNotFoundException);

    Employee guard(2, "Bob", factory_address(), factory_contact(),
                   "guard", Money(500.0), 1);
    REQUIRE(guard.role() == EmployeeRole::SecurityGuard);
    guard.deactivate();
    REQUIRE_FALSE(guard.is_eligible_for_promotion());
}

TEST_CASE("Worker produces units and moves between shifts")
{
    Worker worker(10, "Peter", factory_address(), factory_contact(),
                  Money(800.0), 1, 2, "press", 1);

    REQUIRE_FALSE(worker.meets_quota(10));
    worker.produce(4);
    worker.produce(8);
    REQUIRE(worker.meets_quota(12));
    worker.reset_counter();
    REQUIRE_FALSE(worker.meets_quota(1));

    REQUIRE_THROWS_AS(worker.produce(0), FactoryException);
    REQUIRE_THROWS_AS(worker.produce(-5), FactoryException);

    worker.transfer_to(2, "lathe");
    REQUIRE_THROWS_AS(worker.transfer_to(0, "lathe"), InvalidShiftException);
}

TEST_CASE("Foreman manages a limited team")
{
    Foreman foreman(20, "Boss", factory_address(), factory_contact(),
                    Money(1500.0), 1, 1, 2);

    REQUIRE(foreman.can_accept_more(2));
    foreman.assign_worker(101);
    foreman.assign_worker(102);
    REQUIRE_FALSE(foreman.can_accept_more(2));
    REQUIRE_THROWS_AS(foreman.assign_worker(103), CapacityExceededException);

    Foreman duplicate_foreman(21, "Other boss", factory_address(), factory_contact(),
                              Money(1500.0), 1, 1, 2);
    duplicate_foreman.assign_worker(101);
    REQUIRE_THROWS_AS(duplicate_foreman.assign_worker(101), DuplicateEntityException);

    REQUIRE_FALSE(foreman.remove_worker(999));
    REQUIRE(foreman.remove_worker(101));
    REQUIRE(foreman.can_accept_more(2));

    REQUIRE(foreman.evaluate_team_output(80, 100) == Catch::Approx(80.0));
    REQUIRE(foreman.evaluate_team_output(80, 0) == 0.0);
}

TEST_CASE("Engineer tracks serviced machines")
{
    Engineer engineer(30, "Eve", factory_address(), factory_contact(),
                      Money(1800.0), 1, 3, "mechanical");

    REQUIRE(engineer.service_load() == 0);
    REQUIRE_FALSE(engineer.can_service_machine(7));
    engineer.add_machine(7);
    REQUIRE(engineer.service_load() == 1);
    REQUIRE(engineer.can_service_machine(7));
    REQUIRE_THROWS_AS(engineer.add_machine(7), DuplicateEntityException);
}

TEST_CASE("Accountant calculates tax and balance")
{
    Accountant accountant(40, "Ann", factory_address(), factory_contact(),
                          Money(2000.0), 1, 4, "general", 20.0);

    REQUIRE(accountant.calculate_tax(Money(1000.0)).value() == Catch::Approx(200.0));
    REQUIRE(accountant.net_pay(Money(1000.0)).value() == Catch::Approx(800.0));
    REQUIRE(accountant.is_balanced(Money(1000.0), Money(900.0)));
    REQUIRE_FALSE(accountant.is_balanced(Money(1000.0), Money(1100.0)));
}

TEST_CASE("Storekeeper receives shipments")
{
    Storekeeper storekeeper(50, "Sam", factory_address(), factory_contact(),
                            Money(900.0), 1, 5, 1);

    REQUIRE(storekeeper.processed_units() == 0);
    storekeeper.receive_shipment(25);
    REQUIRE(storekeeper.processed_units() == 25);
    REQUIRE_THROWS_AS(storekeeper.receive_shipment(0), FactoryException);
    REQUIRE(storekeeper.can_store(10, 20));
    REQUIRE_FALSE(storekeeper.can_store(30, 20));
    REQUIRE_FALSE(storekeeper.can_store(0, 20));
}

TEST_CASE("QualityInspector records inspections and certificates")
{
    QualityInspector inspector(60, "Quinn", factory_address(), factory_contact(),
                               Money(1100.0), 1, 6, "electronics");

    REQUIRE(inspector.can_inspect_more(2));
    inspector.record_inspection();
    inspector.record_inspection();
    REQUIRE_FALSE(inspector.can_inspect_more(2));
    REQUIRE(inspector.is_certified_for("electronic"));
    REQUIRE_FALSE(inspector.is_certified_for("food"));
}

TEST_CASE("SecurityGuard logs incidents")
{
    SecurityGuard guard(70, "Greg", factory_address(), factory_contact(),
                        Money(700.0), 1, 7, "gate");

    REQUIRE_FALSE(guard.has_open_incident());
    guard.log_incident("Broken fence");
    guard.log_incident("Unlocked door");
    REQUIRE(guard.incident_count() == 2);
    REQUIRE(guard.has_open_incident());
    REQUIRE_THROWS_AS(guard.log_incident(""), SafetyViolationException);
}

TEST_CASE("Shift validates hours, overlap and staffing")
{
    const Shift morning(1, "morning", 8, 16, 3);
    const Shift evening(2, "evening", 16, 24, 2);
    const Shift night(3, "night", 0, 8, 1);

    REQUIRE(morning.duration_hours() == 8);
    REQUIRE(morning.covers_hour(8));
    REQUIRE_FALSE(morning.covers_hour(16));
    REQUIRE_FALSE(morning.overlaps(evening));
    REQUIRE(morning.overlaps(Shift(4, "overlap", 15, 20, 1)));
    REQUIRE_FALSE(night.overlaps(morning));
    REQUIRE_FALSE(morning.is_fully_staffed(2));
    REQUIRE(morning.is_fully_staffed(3));
    REQUIRE(morning.missing_workers(1) == 2);
    REQUIRE(morning.missing_workers(5) == 0);

    REQUIRE_THROWS_AS(Shift(5, "broken", 8, 8, 1), InvalidShiftException);
    REQUIRE_THROWS_AS(Shift(6, "broken", -1, 8, 1), InvalidShiftException);
    REQUIRE_THROWS_AS(Shift(7, "broken", 20, 25, 1), InvalidShiftException);
}

TEST_CASE("Department manages headcount and budget")
{
    Department department(1, "Assembly", 10, Money(1000.0));

    REQUIRE(department.id() == 1);
    REQUIRE(department.has_manager());
    REQUIRE(department.headcount() == 0);

    department.add_employee(101);
    department.add_employee(102);
    REQUIRE(department.headcount() == 2);
    REQUIRE_THROWS_AS(department.add_employee(101), DuplicateEntityException);
    REQUIRE_THROWS_AS(department.add_employee(0), EntityNotFoundException);

    REQUIRE(department.remaining_budget(Money(400.0)).value() == Catch::Approx(600.0));
    REQUIRE_THROWS_AS(department.remaining_budget(Money(1100.0)), CapacityExceededException);

    REQUIRE_FALSE(department.remove_employee(999));
    REQUIRE(department.remove_employee(101));
    REQUIRE(department.headcount() == 1);

    department.replace_manager(20);
    REQUIRE(department.has_manager());
    REQUIRE_THROWS_AS(department.replace_manager(0), EntityNotFoundException);

    Department without_manager(2, "Temporary", 0, Money(10.0));
    REQUIRE_FALSE(without_manager.has_manager());
}

TEST_CASE("AttendanceRecord tracks hours and overtime")
{
    AttendanceRecord record(1, 5, 8);

    REQUIRE(record.is_valid());
    REQUIRE_FALSE(record.is_overtime(8));
    REQUIRE(record.overtime_hours(8) == 0);

    record.mark_present(10);
    REQUIRE(record.is_overtime(8));
    REQUIRE(record.overtime_hours(8) == 2);

    record.mark_absent();
    REQUIRE(record.is_valid());
    REQUIRE_FALSE(record.is_overtime(8));
    REQUIRE(record.overtime_hours(8) == 0);
    REQUIRE_THROWS_AS(record.mark_present(17), FactoryException);
    REQUIRE_THROWS_AS(record.mark_present(-1), FactoryException);

    const AttendanceRecord invalid(0, 5, 8);
    REQUIRE_FALSE(invalid.is_valid());
}

TEST_CASE("PayrollEntry applies deductions and payments")
{
    PayrollEntry entry(1, Money(1000.0), 20);

    REQUIRE(entry.net().value() == 1000.0);
    REQUIRE(entry.daily_rate().value() == Catch::Approx(50.0));
    REQUIRE(entry.is_consistent());

    REQUIRE(entry.apply_deduction(Money(100.0)).value() == Catch::Approx(100.0));
    REQUIRE(entry.net().value() == Catch::Approx(900.0));
    REQUIRE_THROWS_AS(entry.apply_deduction(Money(1000.0)), InvalidMoneyException);

    entry.mark_paid();
    REQUIRE(entry.is_consistent());

    PayrollEntry invalid(0, Money(1000.0), 0);
    REQUIRE_FALSE(invalid.is_consistent());
    REQUIRE_THROWS_AS(invalid.mark_paid(), InvalidOrderStateException);
}
