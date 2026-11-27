#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "core/Address.hpp"
#include "core/ContactInfo.hpp"
#include "core/DatePeriod.hpp"
#include "core/Money.hpp"
#include "core/enums.hpp"
#include "core/exceptions.hpp"
#include "factory/AuditLog.hpp"
#include "factory/EnergyMeter.hpp"
#include "factory/Factory.hpp"
#include "factory/MaintenanceRequest.hpp"
#include "factory/Notification.hpp"
#include "factory/Report.hpp"
#include "factory/SafetyInspection.hpp"
#include "factory/Tool.hpp"
#include "hr/Department.hpp"
#include "hr/Employee.hpp"
#include "production/Machine.hpp"
#include "production/ProductionLine.hpp"
#include "production/Warehouse.hpp"
#include "production/WorkOrder.hpp"

namespace
{
Address factory_address()
{
    return Address("Factory street", "Minsk", "220000", "Belarus", "1");
}

ContactInfo factory_contact()
{
    return ContactInfo("+375291234567", "worker@factory.by", "office 1");
}

Warehouse factory_warehouse()
{
    return Warehouse(1, "Main", factory_address(), 100);
}

Employee factory_employee(int id, Money salary, int skill)
{
    return Employee(id, "Employee " + std::to_string(id), factory_address(),
                    factory_contact(), "worker", salary, 1, 1, skill);
}
} // namespace

TEST_CASE("Factory aggregates departments and employees")
{
    Factory factory("Minsk Plant", factory_address(), factory_warehouse(), Money(10000.0));

    REQUIRE(factory.employee_count() == 0);
    REQUIRE(factory.total_payroll().value() == 0.0);
    REQUIRE(factory.overall_efficiency() == 0.0);
    REQUIRE(factory.find_department(1) == nullptr);

    Department assembly(1, "Assembly", 10, Money(5000.0));
    assembly.add_employee(101);
    factory.add_department(assembly);
    REQUIRE(factory.find_department(1) != nullptr);
    REQUIRE(factory.find_department(1)->headcount() == 1);
    REQUIRE_THROWS_AS(factory.add_department(assembly), DuplicateEntityException);

    factory.hire(factory_employee(101, Money(1000.0), 1));
    factory.hire(factory_employee(102, Money(2000.0), 5));
    REQUIRE(factory.employee_count() == 2);
    REQUIRE(factory.total_payroll().value() == Catch::Approx(3000.0));
    REQUIRE(factory.overall_efficiency() == Catch::Approx(0.3));
    REQUIRE_THROWS_AS(factory.hire(factory_employee(101, Money(500.0), 1)),
                      DuplicateEntityException);

    WorkOrder draft(1, 1, 10, 1, 5);
    ProductionLine line(1, "Line", 10.0);
    REQUIRE_FALSE(factory.can_produce(draft));
    REQUIRE_THROWS_AS(factory.schedule(draft, line), InvalidOrderStateException);

    WorkOrder cancelled(2, 1, 10, 1, 5, OrderStatus::Cancelled);
    REQUIRE_FALSE(factory.can_produce(cancelled));

    WorkOrder completed(3, 1, 10, 1, 5, OrderStatus::Completed);
    REQUIRE_FALSE(factory.can_produce(completed));

    REQUIRE_THROWS_AS(Factory("Bad", factory_address(), factory_warehouse(), Money(0.0)),
                      InvalidMoneyException);
}

TEST_CASE("MaintenanceRequest escalates and completes")
{
    MaintenanceRequest request(1, 10, "Oil leak", Priority::Low, Money(100.0));

    REQUIRE(request.machine_id() == 10);
    REQUIRE_FALSE(request.is_completed());
    REQUIRE_FALSE(request.is_urgent());
    REQUIRE(request.priority_rank() == 1);

    request.escalate();
    REQUIRE(request.priority_rank() == 2);
    REQUIRE_FALSE(request.is_urgent());

    request.escalate();
    REQUIRE(request.priority_rank() == 3);
    REQUIRE(request.is_urgent());

    request.escalate();
    REQUIRE(request.priority_rank() == 4);
    REQUIRE(request.is_urgent());
    request.escalate();
    REQUIRE(request.priority_rank() == 4);

    REQUIRE(request.complete(Money(150.0)).value() == Catch::Approx(50.0));
    REQUIRE(request.is_completed());
    REQUIRE_THROWS_AS(request.complete(Money(150.0)), InvalidOrderStateException);

    MaintenanceRequest fresh(2, 11, "Vibration", Priority::Normal, Money(100.0));
    REQUIRE_THROWS_AS(fresh.complete(Money(0.0)), InvalidMoneyException);

    REQUIRE_THROWS_AS(MaintenanceRequest(3, 10, "", Priority::High, Money(10.0)),
                      FactoryException);
}

TEST_CASE("Tool wears out, sharpens and breaks")
{
    Tool hammer(1, "Hammer", "good", 100);

    REQUIRE(hammer.is_available());
    REQUIRE_FALSE(hammer.needs_replacement());
    REQUIRE(hammer.wear_percent() == 0.0);

    hammer.use(50);
    REQUIRE(hammer.wear_percent() == Catch::Approx(50.0));
    REQUIRE_FALSE(hammer.needs_replacement());
    REQUIRE_THROWS_AS(hammer.use(0), FactoryException);

    hammer.sharpen();
    REQUIRE(hammer.wear_percent() == Catch::Approx(40.0));
    REQUIRE_FALSE(hammer.needs_replacement());

    hammer.use(70);
    REQUIRE(hammer.wear_percent() == Catch::Approx(110.0));
    REQUIRE(hammer.needs_replacement());
    REQUIRE_FALSE(hammer.is_available());
    REQUIRE_THROWS_AS(hammer.use(1), MachineUnavailableException);
    REQUIRE_THROWS_AS(hammer.sharpen(), MachineUnavailableException);

    REQUIRE_THROWS_AS(Tool(2, "Bad", "good", 0), FactoryException);
}

TEST_CASE("EnergyMeter records consumption and cost")
{
    EnergyMeter meter(1, "Zone A", 0.5, 100.0);

    REQUIRE(meter.cost() == Catch::Approx(50.0));
    REQUIRE(meter.consumption_since(100.0) == 0.0);
    REQUIRE(meter.is_peak(100.0));

    meter.record(20.0);
    REQUIRE(meter.cost() == Catch::Approx(60.0));
    REQUIRE(meter.consumption_since(100.0) == Catch::Approx(20.0));
    REQUIRE(meter.is_peak(110.0));
    REQUIRE_FALSE(meter.is_peak(121.0));

    REQUIRE_THROWS_AS(meter.record(-1.0), FactoryException);
    REQUIRE_THROWS_AS(meter.consumption_since(-1.0), InvalidDateRangeException);
    REQUIRE_THROWS_AS(meter.consumption_since(200.0), InvalidDateRangeException);

    meter.reset_reading();
    REQUIRE(meter.cost() == 0.0);
    REQUIRE_THROWS_AS(EnergyMeter(2, "Bad", 0.0), InvalidMoneyException);
}

TEST_CASE("SafetyInspection grades and repeats checks")
{
    SafetyInspection inspection(1, "Assembly hall", 85, "Inspector");

    REQUIRE(inspection.passed());
    REQUIRE_FALSE(inspection.is_critical());
    REQUIRE(inspection.grade() == "B");
    REQUIRE_FALSE(inspection.requires_follow_up());

    inspection.recheck(60);
    REQUIRE_FALSE(inspection.passed());
    REQUIRE_FALSE(inspection.is_critical());
    REQUIRE(inspection.grade() == "C");
    REQUIRE(inspection.requires_follow_up());

    inspection.recheck(40);
    REQUIRE(inspection.is_critical());
    REQUIRE(inspection.grade() == "D");
    REQUIRE(inspection.requires_follow_up());

    inspection.recheck(95);
    REQUIRE(inspection.passed());
    REQUIRE(inspection.grade() == "A");
    REQUIRE_FALSE(inspection.requires_follow_up());

    REQUIRE_THROWS_AS(inspection.recheck(101), SafetyViolationException);
    REQUIRE_THROWS_AS(SafetyInspection(2, "Area", 101, "Inspector"), SafetyViolationException);
    REQUIRE_THROWS_AS(SafetyInspection(3, "Area", -1, "Inspector"), SafetyViolationException);
}

TEST_CASE("Notification tracks priority and read state")
{
    Notification notification(1, "alice", "Line stopped", Priority::Low);

    REQUIRE_FALSE(notification.is_read());
    REQUIRE_FALSE(notification.is_urgent());
    REQUIRE(notification.format() == "[0] alice: Line stopped");

    notification.escalate();
    REQUIRE_FALSE(notification.is_urgent());
    notification.escalate();
    REQUIRE(notification.is_urgent());
    notification.escalate();
    REQUIRE(notification.is_urgent());
    notification.escalate();
    REQUIRE(notification.format() == "[3] alice: Line stopped");

    notification.mark_read();
    REQUIRE(notification.is_read());

    REQUIRE_THROWS_AS(Notification(2, "", "text", Priority::High), FactoryException);
    REQUIRE_THROWS_AS(Notification(3, "bob", "", Priority::High), FactoryException);
}

TEST_CASE("AuditLog records and counts actions")
{
    AuditLog log("alice", 1);

    REQUIRE(log.size() == 1);
    REQUIRE(log.has_actor("alice"));
    REQUIRE_FALSE(log.has_actor("bob"));
    REQUIRE(log.last_entry() == "created");
    REQUIRE(log.count_actions("created") == 1);

    log.record("deploy");
    log.record("deploy");
    REQUIRE(log.size() == 3);
    REQUIRE(log.count_actions("deploy") == 2);
    REQUIRE(log.last_entry() == "deploy");
    REQUIRE(log.count_actions("missing") == 0);

    REQUIRE_THROWS_AS(log.record(""), FactoryException);
}

TEST_CASE("Report calculates profit and margin")
{
    Report report("Monthly", DatePeriod(0, 9), Money(1000.0), Money(600.0), "Accountant");

    REQUIRE(report.profit().value() == Catch::Approx(400.0));
    REQUIRE(report.margin() == Catch::Approx(40.0));
    REQUIRE(report.is_profitable());
    REQUIRE(report.summary().find("Monthly") != std::string::npos);
    REQUIRE(report.summary().find("10") != std::string::npos);

    Report loss("Monthly", DatePeriod(0, 9), Money(1000.0), Money(1200.0), "Accountant");
    REQUIRE(loss.profit().value() == Catch::Approx(-200.0));
    REQUIRE(loss.margin() == Catch::Approx(-20.0));
    REQUIRE_FALSE(loss.is_profitable());

    REQUIRE_THROWS_AS(
        Report("Bad", DatePeriod(10, 0), Money(1000.0), Money(100.0), "Accountant"),
        InvalidDateRangeException);
    REQUIRE_THROWS_AS(
        Report("Bad", DatePeriod(0, 10), Money(0.0), Money(100.0), "Accountant"),
        InvalidMoneyException);
}
