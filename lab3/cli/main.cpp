#include "core/Address.hpp"
#include "core/ContactInfo.hpp"
#include "core/DocumentNumber.hpp"
#include "core/exceptions.hpp"
#include "factory/AuditLog.hpp"
#include "factory/EnergyMeter.hpp"
#include "factory/Factory.hpp"
#include "factory/MaintenanceRequest.hpp"
#include "factory/Notification.hpp"
#include "factory/Report.hpp"
#include "factory/SafetyInspection.hpp"
#include "factory/Tool.hpp"
#include "hr/AttendanceRecord.hpp"
#include "hr/Department.hpp"
#include "hr/Employee.hpp"
#include "production/Machine.hpp"
#include "production/Material.hpp"
#include "production/Product.hpp"
#include "production/ProductionLine.hpp"
#include "production/QualityCheck.hpp"
#include "production/Warehouse.hpp"
#include "production/WorkOrder.hpp"
#include "sales/Contract.hpp"
#include "sales/Customer.hpp"
#include "sales/Invoice.hpp"
#include "sales/SalesOrder.hpp"
#include "sales/Shipment.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace
{
int read_int(const std::string& prompt)
{
    while (true)
    {
        std::cout << prompt;
        std::string value;
        std::getline(std::cin, value);
        try
        {
            return std::stoi(value);
        }
        catch (const std::exception&)
        {
            std::cout << "Введите целое число.\n";
        }
    }
}

std::string read_string(const std::string& prompt)
{
    std::cout << prompt;
    std::string value;
    std::getline(std::cin, value);
    return value;
}

void print_header(const std::string& text)
{
    std::cout << "\n " << text << " \n";
}
}

int main()
{
    const Address plant_address("Заводская", "Минск", "220000", "Беларусь", "12");
    const ContactInfo plant_contact("+375291112233", "office@factory.by", "101");

    Warehouse warehouse(1, "Главный склад", plant_address, 500);
    Factory factory("Завод Прогресс", plant_address, warehouse, Money(250000.0));

    Department assembly(10, "Сборочный цех", 0, Money(80000.0));
    assembly.add_employee(1);
    assembly.add_employee(2);
    factory.add_department(assembly);

    Worker worker(1, "Иван Петров", plant_address, plant_contact, Money(2500.0), 1, 10,
                  "Сборка", 1);
    Foreman foreman(2, "Сергей Иванов", plant_address, plant_contact, Money(4000.0), 1, 10, 5);
    Engineer engineer(3, "Анна Сидорова", plant_address, plant_contact, Money(4500.0), 1, 10,
                      "механика");
    Accountant accountant(4, "Ольга Ким", plant_address, plant_contact, Money(3800.0), 1, 10,
                          "main", 20.0);
    QualityInspector inspector(5, "Павел Орлов", plant_address, plant_contact, Money(3500.0), 1,
                               10, "металл");

    factory.hire(worker);
    factory.hire(foreman);
    factory.hire(engineer);
    factory.hire(accountant);
    factory.hire(inspector);
    foreman.assign_worker(1);
    engineer.add_machine(1);

    Shift first_shift(1, "Первая", 8, 16, 4);
    AttendanceRecord attendance(1, 1, 9);
    PayrollEntry payroll(1, Money(2500.0), 30);
    payroll.apply_deduction(Money(250.0));

    ProductSpecification specification("BRACKET-A", 1.2, 20, {"сталь", "краска"}, 0.5);
    Product bracket(100, "Кронштейн", specification, Money(35.0), 120);

    Material steel(200, "Сталь", "кг", 500, Money(2.5), 100);
    Supplier supplier(300, "МеталлСнаб", plant_address, plant_contact, 5, 8.5);
    InventoryItem steel_stock(steel, "A-01", 500);
    warehouse.store(steel_stock);

    Machine press(1, "Пресс", 0.92, 1000, 1);
    Machine cutter(2, "Резак", 0.88, 800, 1);
    press.start();
    press.record_operation(120);

    ProductionLine line(1, "Линия кронштейнов", 60.0);
    line.add_machine(1);
    line.add_machine(2);

    WorkOrder order(500, 100, 300, 1, 30);
    order.confirm();
    order.record_progress(120);

    ProductionPlan plan("Ноябрь", Money(100000.0), 85.0);
    plan.add_order(500);

    QualityCheck quality(700, 100, 50, "Павел Орлов");
    quality.record_defect(2);
    DefectReport defect(800, 100, "high", "Заусенец на кромке");

    Customer customer(900, "ООО Стройка", plant_address, plant_contact, Money(50000.0));
    SalesOrder sales_order(1000, 900, 1);
    sales_order.add_item(OrderItem(100, 50, Money(35.0)));
    sales_order.confirm();
    sales_order.register_payment(Money(500.0));

    DatePeriod contract_term(1, 365);
    Contract contract(1100, 900, contract_term, Money(1750.0));
    contract.sign();

    Invoice invoice(1200, 1000, Money(1750.0), 30);
    invoice.issue();
    Payment payment(1300, 1200, Money(500.0), "bank");
    payment.confirm();

    Shipment shipment(1400, 1000, "Минск");
    shipment.dispatch(2);
    shipment.deliver(5);

    Tool drill(1500, "Дрель", "good", 500);
    drill.use(40);
    EnergyMeter meter(1600, "Сборочный цех", 0.28, 1200.0);
    meter.record(75.0);
    SafetyInspection safety(1700, "Сборочный цех", 82, "Павел Орлов");
    Notification notification(1800, "Начальник цеха", "План выполнен на 40%", Priority::Normal);
    AuditLog audit("admin", 1);
    audit.record("создан заказ 500");
    Report report("Итоги месяца", DatePeriod(1, 30), Money(150000.0), Money(120000.0), "Ольга Ким");
    MaintenanceRequest maintenance(1900, 1, "Замена ремня", Priority::High, Money(300.0));

    bool running = true;
    while (running)
    {
        print_header("Система управления заводом");
        std::cout << "1. Показать сотрудников и фонд оплаты\n"
                  << "2. Показать производство и склад\n"
                  << "3. Показать продажи и финансы\n"
                  << "4. Показать инфраструктуру завода\n"
                  << "5. Проверить качество партии\n"
                  << "6. Провести аттестацию безопасности\n"
                  << "7. Оформить заявку на ремонт\n"
                  << "0. Выход\n";

        const int choice = read_int("Выбор: ");
        try
        {
            switch (choice)
            {
            case 1:
                print_header("Сотрудники");
                std::cout << "Фонд оплаты: " << factory.total_payroll().value() << "\n";
                std::cout << "Эффективность персонала: " << factory.overall_efficiency() * 100
                          << "%\n";
                std::cout << "Смена " << first_shift.duration_hours() << " ч, не хватает "
                          << first_shift.missing_workers(3) << " чел.\n";
                std::cout << "Переработка: " << attendance.overtime_hours(8) << " ч\n";
                std::cout << "К выплате: " << payroll.net().value() << "\n";
                std::cout << "Налог: " << accountant.calculate_tax(Money(1000.0)).value() << "\n";
                break;
            case 2:
                print_header("Производство");
                std::cout << "Склад: " << warehouse.occupancy() << "/500, стоимость "
                          << warehouse.total_value().value() << "\n";
                std::cout << "Сталь требует закупки: " << (steel.needs_reorder() ? "да" : "нет")
                          << "\n";
                std::cout << "Заказ выполнен на " << order.completion_percent() << "%\n";
                std::cout << "Оценка стоимости: " << order.estimated_cost(bracket).value()
                          << "\n";
                std::cout << "Линия активна: " << (line.is_active() ? "да" : "нет")
                          << ", мощность " << line.capacity_per_hour() << " ед/ч\n";
                std::cout << "Станок требует ТО: " << (press.needs_maintenance() ? "да" : "нет")
                          << ", доступность " << press.availability() * 100 << "%\n";
                std::cout << "План: " << plan.order_count() << " заказов, средний бюджет "
                          << plan.average_order_budget().value() << "\n";
                std::cout << "Поставщик надёжен: " << (supplier.is_reliable() ? "да" : "нет")
                          << "\n";
                break;
            case 3:
                print_header("Продажи и финансы");
                std::cout << "Заказ клиента: " << sales_order.total().value() << ", остаток "
                          << sales_order.outstanding().value() << "\n";
                std::cout << "Клиент может заказать ещё: "
                          << customer.available_credit().value() << "\n";
                std::cout << "Договор подписан: " << (contract.is_signed() ? "да" : "нет")
                          << ", пеня за 10 дней: " << contract.penalty(10).value() << "\n";
                std::cout << "Счёт: " << invoice.balance().value() << ", оплачен: "
                          << (invoice.is_settled() ? "да" : "нет") << "\n";
                std::cout << payment.receipt() << "\n";
                std::cout << "Доставка: " << (shipment.is_delivered() ? "доставлено" : "в пути")
                          << ", дней в пути " << shipment.transit_days() << "\n";
                std::cout << "Отчёт: " << report.summary() << "\n";
                break;
            case 4:
                print_header("Инфраструктура");
                std::cout << "Инструмент износ: " << drill.wear_percent() << "%\n";
                std::cout << "Энергия: " << meter.cost() << " руб.\n";
                std::cout << "Безопасность: " << safety.grade() << ", критично: "
                          << (safety.is_critical() ? "да" : "нет") << "\n";
                std::cout << "Уведомление: " << notification.format() << "\n";
                std::cout << "Журнал: " << audit.last_entry() << " (записей "
                          << audit.size() << ")\n";
                std::cout << "Ремонт: приоритет " << maintenance.priority_rank() << ", срочно: "
                          << (maintenance.is_urgent() ? "да" : "нет") << "\n";
                break;
            case 5:
            {
                print_header("Контроль качества");
                const int sample = read_int("Размер выборки: ");
                const int defects = read_int("Найдено дефектов: ");
                QualityCheck check(2000, 100, sample, "Павел Орлов");
                check.record_defect(defects);
                std::cout << check.summary() << "\n";
                std::cout << "Доля дефектов: " << check.defect_rate() << "%\n";
                std::cout << "Требуется доработка: " << (check.needs_rework() ? "да" : "нет")
                          << "\n";
                DocumentNumber tickets("QC", 1);
                std::cout << "Тикет дефекта: " << defect.ticket_code(tickets) << "\n";
                inspector.record_inspection();
                break;
            }
            case 6:
            {
                print_header("Аттестация безопасности");
                const int score = read_int("Новая оценка (0-100): ");
                safety.recheck(score);
                std::cout << "Оценка: " << safety.grade() << ", пройдено: "
                          << (safety.passed() ? "да" : "нет") << "\n";
                std::cout << "Нужен контроль: "
                          << (safety.requires_follow_up() ? "да" : "нет") << "\n";
                break;
            }
            case 7:
            {
                print_header("Заявка на ремонт");
                const std::string description = read_string("Описание: ");
                MaintenanceRequest request(2100, 1, description, Priority::Low, Money(200.0));
                request.escalate();
                std::cout << "Приоритет после эскалации: " << request.priority_rank() << "\n";
                break;
            }
            case 0:
                running = false;
                break;
            default:
                std::cout << "Неизвестная команда.\n";
                break;
            }
        }
        catch (const FactoryException& error)
        {
            std::cout << "Ошибка: " << error.getMessage() << "\n";
        }
        catch (const std::exception& error)
        {
            std::cout << "Ошибка: " << error.what() << "\n";
        }
    }

    return 0;
}
