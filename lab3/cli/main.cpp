#include "core/Address.hpp"
#include "core/ContactInfo.hpp"
#include "core/DocumentNumber.hpp"
#include "core/Money.hpp"
#include "core/exceptions.hpp"
#include "factory/AuditLog.hpp"
#include "factory/EnergyMeter.hpp"
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

#include <cmath>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace
{
struct InputClosed {};

std::string read_string(const std::string& prompt)
{
    std::cout << prompt;
    std::string value;
    if (!std::getline(std::cin, value))
        throw InputClosed{};
    return value;
}

template <typename Number>
Number read_number(const std::string& prompt, Number minimum, Number maximum)
{
    while (true)
    {
        std::istringstream input(read_string(prompt));
        Number value;
        if ((input >> value) && (input >> std::ws).eof()
            && std::isfinite(static_cast<double>(value))
            && value >= minimum && value <= maximum)
            return value;
        std::cout << "Введите число в диапазоне " << minimum << ".." << maximum << ".\n";
    }
}

int read_int(const std::string& prompt, int minimum = 0,
             int maximum = std::numeric_limits<int>::max())
{
    return read_number(prompt, minimum, maximum);
}

Money read_money(const std::string& prompt)
{
    return Money(read_number(prompt, 0.01, std::numeric_limits<double>::max()));
}

void print_header(const std::string& text)
{
    std::cout << "\n " << text << " \n";
}

const char* yes_no(bool value)
{
    return value ? "да" : "нет";
}

template <typename T>
int select_id(const std::map<int, T>& objects, const std::string& prompt)
{
    const int id = read_int(prompt, 1);
    if (objects.find(id) == objects.end())
        throw EntityNotFoundException("Объект с таким номером не найден");
    return id;
}

using MenuAction = std::pair<std::string, std::function<void()>>;

void run_menu(const std::string& title, const std::vector<MenuAction>& actions)
{
    while (true)
    {
        print_header(title);
        for (std::size_t i = 0; i < actions.size(); ++i)
            std::cout << i + 1 << ". " << actions[i].first << "\n";
        std::cout << "0. Назад\n";
        const int choice = read_int("Выбор: ", 0, static_cast<int>(actions.size()));
        if (choice == 0)
            return;
        try
        {
            actions[choice - 1].second();
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
}
}

int main() try
{
    const Address plant_address("Заводская", "Минск", "220000", "Беларусь", "12");
    const ContactInfo plant_contact("+375291112233", "office@factory.by", "101");
    Warehouse warehouse(1, "Главный склад", plant_address, 500);
    Department department(10, "Сборочный цех", 0, Money(80000.0));
    std::map<int, Employee> employees;
    for (const auto& employee : std::vector<Employee>{
             Worker(1, "Иван Петров", plant_address, plant_contact, Money(2500), 1, 10, "Сборка", 1),
             Foreman(2, "Сергей Иванов", plant_address, plant_contact, Money(4000), 1, 10, 5),
             Engineer(3, "Анна Сидорова", plant_address, plant_contact, Money(4500), 1, 10, "механика"),
             Accountant(4, "Ольга Ким", plant_address, plant_contact, Money(3800), 1, 10, "main", 20),
             QualityInspector(5, "Павел Орлов", plant_address, plant_contact, Money(3500), 1, 10, "металл"),
             Storekeeper(6, "Алексей Смирнов", plant_address, plant_contact, Money(2800), 1, 10, 1),
             SecurityGuard(7, "Михаил Волков", plant_address, plant_contact, Money(2600), 1, 10, "Проходная")})
    {
        employees.emplace(employee.employee_id(), employee);
        department.add_employee(employee.employee_id());
    }
    Accountant accountant(4, "Ольга Ким", plant_address, plant_contact, Money(3800), 1, 10, "main", 20);
    Shift first_shift(1, "Первая", 8, 16, 4);
    AttendanceRecord attendance(1, 1, 9);
    PayrollEntry payroll(1, Money(2500), 30);
    payroll.apply_deduction(Money(250));

    ProductSpecification specification("BRACKET-A", 1.2, 20, {"сталь", "краска"}, 0.5);
    Product bracket(100, "Кронштейн", specification, Money(35), 120);
    std::map<int, Material> materials;
    materials.emplace(200, Material(200, "Сталь", "кг", 500, Money(2.5), 100));
    warehouse.store(InventoryItem(materials.at(200), "A-01", 500));
    Supplier supplier(300, "МеталлСнаб", plant_address, plant_contact, 5, 8.5);
    Machine press(1, "Пресс", 0.92, 1000, 1);
    Machine cutter(2, "Резак", 0.88, 800, 1);
    press.start();
    press.record_operation(120);
    std::map<int, ProductionLine> lines;
    lines.emplace(1, ProductionLine(1, "Линия кронштейнов", 60));
    lines.at(1).add_machine(1);
    lines.at(1).add_machine(2);
    std::map<int, WorkOrder> work_orders;
    work_orders.emplace(500, WorkOrder(500, 100, 300, 1, 30));
    work_orders.at(500).confirm();
    work_orders.at(500).record_progress(120);
    ProductionPlan plan("Ноябрь", Money(100000), 85);
    plan.add_order(500);
    std::vector<QualityCheck> quality_checks;
    quality_checks.emplace_back(700, 100, 50, employees.at(5).full_name());
    quality_checks.back().record_defect(2);
    std::vector<DefectReport> defects;
    defects.emplace_back(800, 100, "high", "Заусенец на кромке");

    std::map<int, Customer> customers;
    customers.emplace(900, Customer(900, "ООО Стройка", plant_address, plant_contact, Money(50000)));
    std::map<int, SalesOrder> sales_orders;
    sales_orders.emplace(1000, SalesOrder(1000, 900, 1));
    sales_orders.at(1000).add_item(OrderItem(100, 50, Money(35)));
    sales_orders.at(1000).confirm();
    sales_orders.at(1000).register_payment(Money(500));
    Contract contract(1100, 900, DatePeriod(1, 365), Money(1750));
    contract.sign();
    std::map<int, Invoice> invoices;
    invoices.emplace(1200, Invoice(1200, 1000, Money(1750), 30));
    invoices.at(1200).issue();
    invoices.at(1200).apply_payment(Money(500));
    std::map<int, int> invoice_orders{{1200, 1000}};
    std::vector<Payment> payments;
    payments.emplace_back(1300, 1200, Money(500), "bank");
    payments.back().confirm();
    Shipment shipment(1400, 1000, "Минск");
    shipment.dispatch(2);
    shipment.deliver(5);

    Tool drill(1500, "Дрель", "good", 500);
    drill.use(40);
    EnergyMeter meter(1600, "Сборочный цех", 0.28, 1200);
    meter.record(75);
    SafetyInspection safety(1700, "Сборочный цех", 82, "Павел Орлов");
    Notification notification(1800, "Начальник цеха", "План выполнен на 40%", Priority::Normal);
    AuditLog audit("admin", 1);
    audit.record("создан заказ 500");
    Report report("Итоги месяца", DatePeriod(1, 30), Money(150000), Money(120000), "Ольга Ким");
    std::map<int, MaintenanceRequest> maintenance_requests;
    maintenance_requests.emplace(1900, MaintenanceRequest(1900, 1, "Замена ремня", Priority::High, Money(300)));
    std::map<int, std::string> maintenance_descriptions{{1900, "Замена ремня"}};
    DocumentNumber tickets("QC", 1);
    int next_id = 2000;

    const auto print_employees = [&] {
        Money total;
        int skills = 0;
        int active = 0;
        for (const auto& [id, employee] : employees)
        {
            std::cout << id << ". " << employee.full_name() << ", оклад " << employee.salary().value()
                      << ", квалификация " << employee.skill_level()
                      << ", активен: " << yes_no(employee.is_active()) << "\n";
            if (employee.is_active())
            {
                total = total + employee.salary();
                skills += employee.skill_level();
                ++active;
            }
        }
        std::cout << "Фонд оплаты: " << total.value() << "\nЭффективность персонала: "
                  << (active ? 100.0 * skills / (active * 10.0) : 0.0) << "%\n";
    };
    const auto select_employee = [&]() -> Employee& {
        print_employees();
        Employee& employee = employees.at(select_id(employees, "Номер сотрудника: "));
        if (!employee.is_active())
            throw EntityNotFoundException("Сотрудник уже уволен");
        return employee;
    };
    const auto print_materials = [&] {
        for (const auto& [id, material] : materials)
            std::cout << id << ". Остаток: " << material.quantity()
                      << ", доступно: " << warehouse.find(id)->available() << "\n";
    };

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
                  << "8. Сотрудники\n9. Склад\n10. Производство\n11. Продажи\n12. Ремонт\n0. Выход\n";
        const int choice = read_int("Выбор: ");
        try
        {
            switch (choice)
            {
            case 1:
                print_header("Сотрудники");
                print_employees();
                std::cout << "Смена " << first_shift.duration_hours() << " ч, не хватает "
                          << first_shift.missing_workers(3) << " чел.\n"
                          << "Переработка: " << attendance.overtime_hours(8) << " ч\n"
                          << "К выплате: " << payroll.net().value() << "\n"
                          << "Налог: " << accountant.calculate_tax(Money(1000)).value() << "\n";
                break;
            case 2:
                print_header("Производство");
                std::cout << "Склад: " << warehouse.occupancy() << "/500, стоимость "
                          << warehouse.total_value().value() << "\n"
                          << "Сталь требует закупки: " << yes_no(materials.at(200).needs_reorder()) << "\n";
                print_materials();
                for (const auto& [id, order] : work_orders)
                    std::cout << "Заказ #" << id << " выполнен на " << order.completion_percent()
                              << "%, оценка стоимости: " << order.estimated_cost(bracket).value() << "\n";
                for (const auto& [id, line] : lines)
                    std::cout << "Линия #" << id << " активна: " << yes_no(line.is_active())
                              << ", мощность " << line.capacity_per_hour() << " ед/ч\n";
                std::cout << "Станок требует ТО: " << yes_no(press.needs_maintenance())
                          << ", доступность " << press.availability() * 100 << "%\n"
                          << "План: " << plan.order_count() << " заказов, средний бюджет "
                          << plan.average_order_budget().value() << "\n"
                          << "Поставщик надёжен: " << yes_no(supplier.is_reliable()) << "\n";
                break;
            case 3:
                print_header("Продажи и финансы");
                for (const auto& [id, order] : sales_orders)
                    std::cout << "Заказ #" << id << ", клиент #" << order.customer_id()
                              << ": " << order.total().value() << ", остаток " << order.outstanding().value() << "\n";
                for (const auto& [id, customer] : customers)
                    std::cout << "Клиент #" << id << " " << customer.name()
                              << " может заказать ещё: " << customer.available_credit().value() << "\n";
                std::cout << "Договор подписан: " << yes_no(contract.is_signed())
                          << ", пеня за 10 дней: " << contract.penalty(10).value() << "\n";
                for (const auto& [id, invoice] : invoices)
                    std::cout << "Счёт #" << id << ": " << invoice.balance().value()
                              << ", оплачен: " << yes_no(invoice.is_settled()) << "\n";
                for (const auto& payment : payments)
                    std::cout << payment.receipt() << "\n";
                std::cout << "Доставка: " << (shipment.is_delivered() ? "доставлена" : "подготовлена")
                          << ", дней в пути " << shipment.transit_days() << "\n"
                          << "Отчёт: " << report.summary() << "\n";
                break;
            case 4:
                print_header("Инфраструктура");
                std::cout << "Инструмент износ: " << drill.wear_percent() << "%\n"
                          << "Энергия: " << meter.cost() << " руб.\n"
                          << "Безопасность: " << safety.grade() << ", критично: " << yes_no(safety.is_critical()) << "\n"
                          << "Уведомление: " << notification.format() << "\n"
                          << "Журнал: " << audit.last_entry() << " (записей " << audit.size() << ")\n";
                for (const auto& [id, request] : maintenance_requests)
                    std::cout << "Ремонт #" << id << ": приоритет " << request.priority_rank()
                              << ", срочно: " << yes_no(request.is_urgent()) << "\n";
                break;
            case 5:
            {
                print_header("Контроль качества");
                const int sample = read_int("Размер выборки: ", 1);
                const int count = read_int("Найдено дефектов: ", 0, sample);
                quality_checks.emplace_back(next_id++, 100, sample, employees.at(5).full_name());
                auto& quality = quality_checks.back();
                if (count > 0)
                    quality.record_defect(count);
                std::cout << quality.summary() << "\nДоля дефектов: " << quality.defect_rate()
                          << "%\nТребуется доработка: " << yes_no(quality.needs_rework()) << "\n";
                if (count > 0)
                {
                    defects.emplace_back(next_id++, 100, "high", "Дефекты при проверке партии");
                    std::cout << "Тикет дефекта: " << defects.back().ticket_code(tickets) << "\n";
                }
                audit.record("проверка качества");
                break;
            }
            case 6:
                print_header("Аттестация безопасности");
                safety.recheck(read_int("Новая оценка (0-100): ", 0, 100));
                std::cout << "Оценка: " << safety.grade() << ", пройдено: " << yes_no(safety.passed())
                          << "\nНужен контроль: " << yes_no(safety.requires_follow_up()) << "\n";
                break;
            case 7:
            {
                print_header("Заявка на ремонт");
                const std::string description = read_string("Описание: ");
                const int machine_id = read_int("Станок (1 — пресс, 2 — резак): ", 1, 2);
                const int priority = read_int("Приоритет (1 — низкий, 2 — обычный, 3 — высокий, 4 — критический): ", 1, 4);
                const Money estimate = read_money("Оценка стоимости: ");
                const int id = next_id++;
                maintenance_requests.emplace(id, MaintenanceRequest(id, machine_id, description,
                                             static_cast<Priority>(priority - 1), estimate));
                maintenance_descriptions.emplace(id, description);
                audit.record("создана заявка на ремонт");
                std::cout << "Заявка #" << id << " сохранена. Приоритет: "
                          << maintenance_requests.at(id).priority_rank() << "\n";
                break;
            }
            case 8:
                run_menu("Сотрудники", {
                    {"Принять сотрудника", [&] {
                        const std::string name = read_string("ФИО: ");
                        const std::string position = read_string("Должность: ");
                        const Money salary = read_money("Оклад: ");
                        const int day = read_int("День приёма: ");
                        const int id = next_id++;
                        employees.emplace(id, Employee(id, name, plant_address, plant_contact,
                                                      position, salary, day, department.id()));
                        department.add_employee(id);
                        std::cout << "Сотрудник принят. Номер: " << id << "\n";
                    }},
                    {"Уволить сотрудника", [&] {
                        Employee& employee = select_employee();
                        department.remove_employee(employee.employee_id());
                        employee.deactivate();
                        std::cout << "Сотрудник уволен.\n";
                    }},
                    {"Повысить оклад", [&] {
                        Employee& employee = select_employee();
                        const double percent = read_number("Повышение, %: ", 0.01, std::numeric_limits<double>::max());
                        std::cout << "Новый оклад: " << employee.give_raise(percent).value() << "\n";
                    }},
                    {"Показать список", print_employees}
                });
                break;
            case 9:
                run_menu("Склад", {
                    {"Добавить материал", [&] {
                        const std::string name = read_string("Название: ");
                        const std::string unit = read_string("Единица измерения: ");
                        const int quantity = read_int("Количество: ", 1);
                        const Money cost = read_money("Стоимость единицы: ");
                        const int reorder = read_int("Минимальный остаток: ");
                        const std::string location = read_string("Место хранения: ");
                        if (!warehouse.can_accept(1))
                            throw CapacityExceededException("Склад заполнен");
                        const int id = next_id++;
                        materials.emplace(id, Material(id, name, unit, quantity, cost, reorder));
                        warehouse.store(InventoryItem(materials.at(id), location, quantity));
                        std::cout << "Материал добавлен. Номер: " << id << "\n";
                    }},
                    {"Списать материал", [&] {
                        print_materials();
                        const int id = select_id(materials, "Номер материала: ");
                        const int units = read_int("Количество: ", 1);
                        if (units > warehouse.find(id)->available())
                            throw InsufficientStockException("Недостаточно свободного материала");
                        // Складская модель не удаляет единицы: резерв отражает их выбытие.
                        warehouse.dispatch(id, units);
                        materials.at(id).consume(units);
                        std::cout << "Материал списан; на складе соответствующий остаток зарезервирован.\n";
                    }},
                    {"Отгрузить материал", [&] {
                        print_materials();
                        const int id = select_id(materials, "Номер материала: ");
                        const int units = read_int("Количество: ", 1);
                        warehouse.dispatch(id, units);
                        materials.at(id).consume(units);
                        std::cout << "Отгрузка учтена резервом склада.\n";
                    }}
                });
                break;
            case 10:
                run_menu("Производство", {
                    {"Создать заказ", [&] {
                        const int units = read_int("Количество кронштейнов: ", 1);
                        const int day = read_int("Срок исполнения, день: ");
                        const int id = next_id++;
                        work_orders.emplace(id, WorkOrder(id, bracket.id(), units, 1, day));
                        work_orders.at(id).confirm();
                        plan.add_order(id);
                        std::cout << "Заказ #" << id << " создан и подтверждён.\n";
                    }},
                    {"Запустить линию", [&] {
                        for (const auto& [id, line] : lines)
                            std::cout << id << ". Активна: " << yes_no(line.is_active()) << "\n";
                        const int id = select_id(lines, "Номер линии: ");
                        lines.at(id).activate();
                        std::cout << "Линия запущена.\n";
                    }},
                    {"Зафиксировать прогресс", [&] {
                        for (const auto& [id, order] : work_orders)
                            std::cout << id << ". Выполнено: " << order.completion_percent() << "%\n";
                        auto& order = work_orders.at(select_id(work_orders, "Номер заказа: "));
                        if (order.remaining_quantity() == 0)
                            throw InvalidOrderStateException("Заказ уже выполнен");
                        order.record_progress(read_int("Выпущено изделий: ", 1, order.remaining_quantity()));
                        std::cout << "Выполнено: " << order.completion_percent() << "%\n";
                    }}
                });
                break;
            case 11:
                run_menu("Продажи", {
                    {"Создать клиента", [&] {
                        const std::string name = read_string("Название клиента: ");
                        const Money credit = read_money("Кредитный лимит: ");
                        const int id = next_id++;
                        customers.emplace(id, Customer(id, name, plant_address, plant_contact, credit));
                        std::cout << "Клиент создан. Номер: " << id << "\n";
                    }},
                    {"Создать заказ", [&] {
                        for (const auto& [id, customer] : customers)
                            std::cout << id << ". " << customer.name() << "\n";
                        const int customer_id = select_id(customers, "Номер клиента: ");
                        const int units = read_int("Количество кронштейнов: ", 1);
                        const int day = read_int("День создания: ");
                        const int id = next_id++;
                        sales_orders.emplace(id, SalesOrder(id, customer_id, day));
                        sales_orders.at(id).add_item(OrderItem(bracket.id(), units, bracket.unit_price()));
                        sales_orders.at(id).confirm();
                        std::cout << "Заказ #" << id << " создан, сумма: " << sales_orders.at(id).total().value() << "\n";
                    }},
                    {"Выставить счёт", [&] {
                        for (const auto& [id, order] : sales_orders)
                            std::cout << id << ". Сумма: " << order.total().value() << "\n";
                        const int order_id = select_id(sales_orders, "Номер заказа: ");
                        for (const auto& [id, linked_order] : invoice_orders)
                            if (linked_order == order_id)
                                throw DuplicateEntityException("Счёт по этому заказу уже выставлен");
                        const int day = read_int("Срок оплаты, день: ");
                        const int id = next_id++;
                        invoices.emplace(id, Invoice(id, order_id, sales_orders.at(order_id).total(), day));
                        invoices.at(id).issue();
                        invoice_orders.emplace(id, order_id);
                        std::cout << "Счёт #" << id << " выставлен.\n";
                    }},
                    {"Принять оплату", [&] {
                        for (const auto& [id, invoice] : invoices)
                            std::cout << id << ". Остаток: " << invoice.balance().value() << "\n";
                        const int id = select_id(invoices, "Номер счёта: ");
                        auto& invoice = invoices.at(id);
                        auto& order = sales_orders.at(invoice_orders.at(id));
                        const Money amount = read_money("Сумма платежа: ");
                        const std::string method = read_string("Способ (bank/cash/card): ");
                        if (amount.value() > invoice.balance().value() || amount.value() > order.outstanding().value())
                            throw InvalidMoneyException("Платёж превышает остаток");
                        payments.emplace_back(next_id++, id, amount, method);
                        payments.back().confirm();
                        invoice.apply_payment(amount);
                        order.register_payment(amount);
                        std::cout << payments.back().receipt() << "\nОстаток счёта: " << invoice.balance().value() << "\n";
                    }}
                });
                break;
            case 12:
                run_menu("Ремонт", {
                    {"Показать список заявок", [&] {
                        for (const auto& [id, request] : maintenance_requests)
                            std::cout << id << ". Станок #" << request.machine_id() << ", "
                                      << maintenance_descriptions.at(id) << ", приоритет " << request.priority_rank()
                                      << ", закрыта: " << yes_no(request.is_completed()) << "\n";
                    }},
                    {"Закрыть заявку", [&] {
                        const int id = select_id(maintenance_requests, "Номер заявки: ");
                        auto& request = maintenance_requests.at(id);
                        if (request.is_completed())
                            throw InvalidOrderStateException("Заявка уже закрыта");
                        const Money actual = read_money("Фактическая стоимость: ");
                        std::cout << "Отклонение от сметы: " << request.complete(actual).value() << "\n";
                        audit.record("заявка на ремонт закрыта");
                    }}
                });
                break;
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
catch (const InputClosed&)
{
    std::cout << "\nВвод завершён.\n";
    return 0;
}
