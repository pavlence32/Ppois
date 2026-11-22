#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "core/Address.hpp"
#include "core/ContactInfo.hpp"
#include "core/DocumentNumber.hpp"
#include "core/Money.hpp"
#include "core/enums.hpp"
#include "core/exceptions.hpp"
#include "production/Machine.hpp"
#include "production/Material.hpp"
#include "production/Product.hpp"
#include "production/ProductionLine.hpp"
#include "production/QualityCheck.hpp"
#include "production/Warehouse.hpp"
#include "production/WorkOrder.hpp"

namespace
{
Address warehouse_address()
{
    return Address("Industrial street", "Minsk", "220000", "Belarus", "1");
}

ContactInfo supplier_contact()
{
    return ContactInfo("+375291234567", "sales@supplier.by", "office 1");
}

ProductSpecification widget_spec()
{
    return ProductSpecification("W-1", 2.0, 30, {"Steel", "Paint"}, 10.0);
}

Product widget()
{
    return Product(10, "Widget", widget_spec(), Money(10.0), 5);
}
} // namespace

TEST_CASE("ProductSpecification validates measurements and materials")
{
    const ProductSpecification specification = widget_spec();

    REQUIRE(specification.is_valid());
    REQUIRE(specification.is_within_tolerance(2.005));
    REQUIRE_FALSE(specification.is_within_tolerance(2.02));
    REQUIRE(specification.production_hours(2) == Catch::Approx(1.0));
    REQUIRE(specification.production_hours(0) == 0.0);
    REQUIRE(specification.requires_material("Steel"));
    REQUIRE_FALSE(specification.requires_material("Wood"));

    const ProductSpecification invalid("", 0.0, 0, {}, 0.0);
    REQUIRE_FALSE(invalid.is_valid());
}

TEST_CASE("Product manages stock and specification matching")
{
    Product product = widget();

    REQUIRE(product.id() == 10);
    REQUIRE(product.stock() == 5);
    REQUIRE(product.unit_price().value() == 10.0);
    REQUIRE(product.is_available(5));
    REQUIRE_FALSE(product.is_available(6));
    REQUIRE_FALSE(product.is_available(0));
    REQUIRE(product.inventory_value().value() == Catch::Approx(50.0));
    REQUIRE(product.matches_specification(widget_spec()));

    product.reserve(3);
    REQUIRE(product.stock() == 2);
    REQUIRE_THROWS_AS(product.reserve(3), InsufficientStockException);
    REQUIRE_THROWS_AS(product.reserve(0), InvalidMoneyException);

    product.restock(8);
    REQUIRE(product.stock() == 10);
    REQUIRE_THROWS_AS(product.restock(0), FactoryException);
}

TEST_CASE("Material consumes, restocks and values inventory")
{
    Material steel(1, "Steel", "kg", 100, Money(5.0), 20);

    REQUIRE(steel.id() == 1);
    REQUIRE(steel.quantity() == 100);
    REQUIRE(steel.total_value().value() == Catch::Approx(500.0));
    REQUIRE_FALSE(steel.needs_reorder());
    REQUIRE(steel.shortage(90) == 0);
    REQUIRE(steel.shortage(130) == 30);
    REQUIRE(steel.valuation(3).value() == Catch::Approx(15.0));

    steel.consume(90);
    REQUIRE(steel.quantity() == 10);
    REQUIRE(steel.needs_reorder());
    REQUIRE_THROWS_AS(steel.consume(11), InsufficientStockException);
    REQUIRE_THROWS_AS(steel.consume(0), FactoryException);

    steel.restock(40);
    REQUIRE(steel.quantity() == 50);
    REQUIRE_THROWS_AS(steel.restock(-1), FactoryException);
    REQUIRE_THROWS_AS(steel.valuation(-1), InvalidMoneyException);
}

TEST_CASE("Supplier evaluates delivery, rating and quotes")
{
    Supplier supplier(1, "Parts Ltd", warehouse_address(), supplier_contact(), 5, 8.5);

    REQUIRE(supplier.can_deliver_within(5));
    REQUIRE(supplier.can_deliver_within(7));
    REQUIRE_FALSE(supplier.can_deliver_within(3));
    REQUIRE(supplier.is_reliable());
    REQUIRE(supplier.serves_city("Minsk"));
    REQUIRE_FALSE(supplier.serves_city("Warsaw"));

    const Material steel(1, "Steel", "kg", 100, Money(5.0), 20);
    REQUIRE(supplier.quote(steel, 10).value() == Catch::Approx(52.5));
    REQUIRE_THROWS_AS(supplier.quote(steel, 0), InvalidMoneyException);

    supplier.adjust_rating(5.0);
    REQUIRE(supplier.quote(steel, 10).value() == Catch::Approx(52.5));
    supplier.adjust_rating(-100.0);
    REQUIRE_FALSE(supplier.is_reliable());
    REQUIRE(supplier.quote(steel, 10).value() == Catch::Approx(50.0));
}

TEST_CASE("InventoryItem tracks reservations and value")
{
    InventoryItem item(Material(1, "Steel", "kg", 100, Money(5.0), 20), "A-1", 40);

    REQUIRE(item.material_id() == 1);
    REQUIRE(item.available() == 40);
    REQUIRE_FALSE(item.below_reorder());
    REQUIRE(item.value().value() == Catch::Approx(200.0));

    item.reserve(10);
    REQUIRE(item.available() == 30);
    REQUIRE_THROWS_AS(item.reserve(31), InsufficientStockException);
    REQUIRE_THROWS_AS(item.reserve(0), InvalidMoneyException);

    item.release(5);
    REQUIRE(item.available() == 35);
    REQUIRE_THROWS_AS(item.release(100), FactoryException);

    item.add(10);
    REQUIRE(item.available() == 45);
    REQUIRE_THROWS_AS(item.add(0), FactoryException);
}

TEST_CASE("Warehouse stores, finds and dispatches materials")
{
    Warehouse warehouse(1, "Main", warehouse_address(), 2);

    REQUIRE(warehouse.free_capacity() == 2);
    REQUIRE(warehouse.occupancy() == 0);
    REQUIRE(warehouse.can_accept(2));
    REQUIRE_FALSE(warehouse.can_accept(3));
    REQUIRE_FALSE(warehouse.can_accept(0));

    warehouse.store(InventoryItem(Material(1, "Steel", "kg", 100, Money(5.0), 20), "A-1", 40));
    warehouse.store(InventoryItem(Material(2, "Paint", "l", 50, Money(3.0), 10), "B-1", 20));

    REQUIRE(warehouse.occupancy() == 2);
    REQUIRE(warehouse.free_capacity() == 0);
    REQUIRE(warehouse.total_value().value() == Catch::Approx(260.0));
    REQUIRE(warehouse.find(1) != nullptr);
    REQUIRE(warehouse.find(1)->material_id() == 1);
    REQUIRE(warehouse.find(99) == nullptr);
    REQUIRE_THROWS_AS(warehouse.store(
                          InventoryItem(Material(3, "Oil", "l", 10, Money(1.0), 1), "C-1", 5)),
                      CapacityExceededException);

    warehouse.dispatch(1, 15);
    REQUIRE(warehouse.find(1)->available() == 25);
    REQUIRE_THROWS_AS(warehouse.dispatch(1, 26), InsufficientStockException);
    REQUIRE_THROWS_AS(warehouse.dispatch(99, 1), EntityNotFoundException);
    REQUIRE_THROWS_AS(warehouse.dispatch(1, 0), InsufficientStockException);

    REQUIRE_THROWS_AS(Warehouse(2, "Broken", warehouse_address(), 0), CapacityExceededException);
}

TEST_CASE("Machine tracks operation, maintenance and breakdown")
{
    Machine machine(1, "Press", 0.9, 10, 1);

    REQUIRE(machine.id() == 1);
    REQUIRE(machine.status() == MachineStatus::Idle);
    REQUIRE_FALSE(machine.needs_maintenance());

    machine.start();
    REQUIRE(machine.status() == MachineStatus::Running);
    machine.record_operation(4);
    REQUIRE(machine.availability() == Catch::Approx(0.828));
    REQUIRE_FALSE(machine.needs_maintenance());

    machine.record_operation(6);
    REQUIRE(machine.needs_maintenance());
    REQUIRE(machine.status() == MachineStatus::UnderMaintenance);
    REQUIRE_THROWS_AS(machine.start(), MachineUnavailableException);

    machine.complete_maintenance();
    REQUIRE(machine.status() == MachineStatus::Idle);
    REQUIRE_FALSE(machine.needs_maintenance());

    machine.start();
    machine.report_breakdown();
    REQUIRE(machine.status() == MachineStatus::Broken);
    REQUIRE(machine.availability() == 0.0);
    REQUIRE_THROWS_AS(machine.start(), MachineUnavailableException);

    REQUIRE_THROWS_AS(machine.record_operation(1), MachineUnavailableException);
    machine.stop();
    REQUIRE(machine.status() == MachineStatus::Broken);

    REQUIRE_THROWS_AS(Machine(2, "Bad", 1.5, 10, 1), FactoryException);
    REQUIRE_THROWS_AS(Machine(2, "Bad", -0.1, 10, 1), FactoryException);
}

TEST_CASE("MaintenanceRecord completes work and tracks overdue")
{
    MaintenanceRecord record(7, 10, "Tech", "Replace bearing", Money(100.0));

    REQUIRE(record.machine_id() == 7);
    REQUIRE_FALSE(record.is_completed());
    REQUIRE(record.is_overdue(20, 5));
    REQUIRE_FALSE(record.is_overdue(15, 5));
    REQUIRE(record.cost_with_parts(Money(25.0)).value() == Catch::Approx(125.0));

    record.complete();
    REQUIRE(record.is_completed());
    REQUIRE_FALSE(record.is_overdue(20, 5));

    MaintenanceRecord empty(8, 10, "Tech", "", Money(10.0));
    REQUIRE_THROWS_AS(empty.complete(), FactoryException);
}

TEST_CASE("ProductionLine requires machines and controls activation")
{
    ProductionLine line(1, "Assembly", 50.0);

    REQUIRE_FALSE(line.can_run());
    REQUIRE_FALSE(line.is_active());
    REQUIRE(line.capacity_per_hour() == 0.0);
    REQUIRE_THROWS_AS(line.activate(), MachineUnavailableException);

    line.add_machine(10);
    REQUIRE_FALSE(line.can_run());
    line.add_machine(11);
    REQUIRE(line.can_run());
    REQUIRE(line.capacity_per_hour() == Catch::Approx(50.0));
    REQUIRE_THROWS_AS(line.add_machine(10), DuplicateEntityException);

    line.activate();
    REQUIRE(line.is_active());
    line.deactivate();
    REQUIRE_FALSE(line.is_active());

    REQUIRE_FALSE(line.remove_machine(99));
    REQUIRE(line.remove_machine(10));
    REQUIRE_FALSE(line.can_run());
    REQUIRE(line.remove_machine(11));
    REQUIRE_FALSE(line.can_run());
    REQUIRE_FALSE(line.is_active());

    REQUIRE_THROWS_AS(ProductionLine(2, "Bad", 0.0), FactoryException);
}

TEST_CASE("WorkOrder progresses through production states")
{
    WorkOrder order(1, 10, 100, 1, 20);
    const Product product = widget();

    REQUIRE(order.status() == OrderStatus::Draft);
    REQUIRE(order.completion_percent() == 0.0);
    REQUIRE(order.remaining_quantity() == 100);
    REQUIRE(order.estimated_cost(product).value() == Catch::Approx(1000.0));
    REQUIRE_FALSE(order.is_complete());
    REQUIRE_THROWS_AS(order.record_progress(10), InvalidOrderStateException);

    order.confirm();
    REQUIRE(order.status() == OrderStatus::Confirmed);
    REQUIRE_THROWS_AS(order.confirm(), InvalidOrderStateException);

    order.record_progress(40);
    REQUIRE(order.status() == OrderStatus::InProduction);
    REQUIRE(order.completion_percent() == Catch::Approx(40.0));
    REQUIRE(order.remaining_quantity() == 60);
    REQUIRE_THROWS_AS(order.record_progress(0), FactoryException);

    order.record_progress(60);
    REQUIRE(order.is_complete());
    REQUIRE(order.status() == OrderStatus::Completed);
    REQUIRE(order.completion_percent() == Catch::Approx(100.0));
    REQUIRE_FALSE(order.is_overdue(100));
    REQUIRE_THROWS_AS(order.cancel(), InvalidOrderStateException);

    WorkOrder late(2, 10, 5, 1, 3, OrderStatus::Confirmed);
    REQUIRE_FALSE(late.is_overdue(3));
    REQUIRE(late.is_overdue(4));

    WorkOrder cancelled(3, 10, 5, 1, 3, OrderStatus::Cancelled);
    REQUIRE_THROWS_AS(cancelled.cancel(), InvalidOrderStateException);

    REQUIRE_THROWS_AS(WorkOrder(4, 10, 0, 1, 3), FactoryException);
}

TEST_CASE("ProductionPlan tracks orders and budget")
{
    ProductionPlan plan("Q1", Money(1000.0), 0.8);

    REQUIRE(plan.order_count() == 0);
    REQUIRE(plan.average_order_budget().value() == 0.0);
    REQUIRE(plan.is_within_budget(Money(500.0)));

    plan.add_order(1);
    plan.add_order(2);
    REQUIRE(plan.order_count() == 2);
    REQUIRE(plan.average_order_budget().value() == Catch::Approx(500.0));
    REQUIRE(plan.is_within_budget(Money(1000.0)));
    REQUIRE_FALSE(plan.is_within_budget(Money(1001.0)));
    REQUIRE(plan.load_percent(4) == Catch::Approx(50.0));
    REQUIRE_THROWS_AS(plan.load_percent(0), CapacityExceededException);

    REQUIRE_THROWS_AS(plan.add_order(1), DuplicateEntityException);
    REQUIRE_THROWS_AS(plan.add_order(0), EntityNotFoundException);

    REQUIRE_FALSE(plan.remove_order(99));
    REQUIRE(plan.remove_order(1));
    REQUIRE(plan.order_count() == 1);
}

TEST_CASE("QualityCheck computes defects and rework")
{
    QualityCheck check(1, 10, 100, "Inspector");

    REQUIRE(check.defect_rate() == 0.0);
    REQUIRE(check.passes(5.0));
    REQUIRE_FALSE(check.needs_rework());
    REQUIRE(check.summary().find("Проверка #1") != std::string::npos);

    check.record_defect(5);
    REQUIRE(check.defect_rate() == Catch::Approx(5.0));
    REQUIRE(check.passes(5.0));
    REQUIRE_FALSE(check.needs_rework());

    check.record_defect(6);
    REQUIRE(check.defect_rate() == Catch::Approx(11.0));
    REQUIRE_FALSE(check.passes(5.0));
    REQUIRE(check.needs_rework());

    REQUIRE_THROWS_AS(check.record_defect(0), FactoryException);
    REQUIRE_THROWS_AS(QualityCheck(2, 10, 0, "Inspector"), FactoryException);

    QualityCheck rework(3, 10, 10, "Inspector", QualityResult::Rework);
    REQUIRE(rework.needs_rework());
}

TEST_CASE("DefectReport resolves issues and issues tickets")
{
    DefectReport report(1, 10, "critical", "Crack in housing");

    REQUIRE(report.is_critical());
    REQUIRE_FALSE(report.is_resolved());
    REQUIRE(report.matches_product(widget()));
    REQUIRE_FALSE(report.matches_product(Product(11, "Other", widget_spec(), Money(1.0))));

    DocumentNumber counter("DEF", 1);
    REQUIRE(report.ticket_code(counter) == "DEF-0001");
    REQUIRE(report.ticket_code(counter) == "DEF-0002");

    report.resolve();
    REQUIRE(report.is_resolved());

    DefectReport minor(2, 10, "low", "Scratch");
    REQUIRE_FALSE(minor.is_critical());

    DefectReport no_description(3, 10, "low", "");
    REQUIRE_THROWS_AS(no_description.resolve(), FactoryException);
}
