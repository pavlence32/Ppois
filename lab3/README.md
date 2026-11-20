# Lab 3 — Система управления заводом

Объектная модель завода на C++17: кадры, производство, склад, качество,
продажи, финансы и инфраструктура. Код разделён по пакетам в каталогах
`include/`, `src/` и `cli/`.

## Сборка и запуск

```bash
cmake -S . -B build
cmake --build build
./build/lab3_cli
```

CLI открывает интерактивное меню: сотрудники и фонд оплаты, производство и
склад, продажи и финансы, инфраструктура, контроль качества, аттестация
безопасности и заявки на ремонт.

## Структура

| Пакет | Назначение |
|---|---|
| `core` | деньги, адреса, контакты, периоды, номера документов, исключения |
| `hr` | люди, сотрудники, роли, смены, подразделения, табель и расчётные листы |
| `production` | изделия, материалы, поставщики, склад, станки, линии, заказы, качество |
| `sales` | клиенты, заказы, договоры, отгрузки, счета и платежи |
| `factory` | сам завод, ремонты, инструмент, энергия, безопасность, журнал, отчёты |

## Выполнение требований

| Требование | Фактически |
|---|---:|
| Классы | 59 (без учёта 7 enum-типов) |
| Поля (суммарно) | 208 |
| Содержательные поведения | 223 |
| Ассоциации между классами | 45+ |
| Собственные исключения | 13 (базовое + 12 наследников) |

Ассоциации — это поля и параметры методов, типизированные другим классом:
`Person::address_`, `Employee::salary_`, `Product::specification_`,
`InventoryItem::material_`, `Warehouse::items_`, `SalesOrder::items_`,
`Contract::term_`, `Factory::departments_`, `Factory::warehouse_`,
`Factory::schedule(WorkOrder&, ProductionLine&)`,
`Supplier::quote(const Material&, int)` и другие.

## Классы

| Класс | Поля | Поведения | Связанные классы |
|---|---:|---:|---|
| Money | 1 | 9 | — |
| Address | 5 | 3 | — |
| DatePeriod | 2 | 5 | — |
| ContactInfo | 3 | 4 | — |
| DocumentNumber | 2 | 4 | — |
| FactoryException | 1 | 1 | — |
| InvalidMoneyException | 0 | — | FactoryException |
| InvalidDateRangeException | 0 | — | FactoryException |
| EntityNotFoundException | 0 | — | FactoryException |
| DuplicateEntityException | 0 | — | FactoryException |
| UnauthorizedAccessException | 0 | — | FactoryException |
| InsufficientStockException | 0 | — | FactoryException |
| MachineUnavailableException | 0 | — | FactoryException |
| QualityCheckFailedException | 0 | — | FactoryException |
| InvalidOrderStateException | 0 | — | FactoryException |
| CapacityExceededException | 0 | — | FactoryException |
| InvalidShiftException | 0 | — | FactoryException |
| SafetyViolationException | 0 | — | FactoryException |
| Person | 5 | 5 | Address, ContactInfo |
| Employee | 6 | 7 | Person, Money |
| Worker | 3 | 4 | Employee, Shift (id) |
| Foreman | 2 | 4 | Employee, Worker (id) |
| Engineer | 2 | 3 | Employee, Machine (id) |
| Accountant | 2 | 3 | Employee, Money |
| Storekeeper | 2 | 2 | Employee, Warehouse (id) |
| QualityInspector | 2 | 3 | Employee, QualityCheck |
| SecurityGuard | 2 | 3 | Employee |
| Shift | 5 | 5 | — |
| Department | 5 | 6 | Money, Employee (id) |
| AttendanceRecord | 4 | 5 | Employee (id) |
| PayrollEntry | 5 | 5 | Employee (id), Money |
| ProductSpecification | 5 | 4 | Material (names) |
| Product | 5 | 5 | ProductSpecification, Money |
| Material | 6 | 6 | Money |
| Supplier | 6 | 5 | Address, ContactInfo, Material, Money |
| InventoryItem | 4 | 6 | Material |
| Warehouse | 5 | 7 | Address, InventoryItem, Money |
| Machine | 7 | 7 | ProductionLine (id) |
| MaintenanceRecord | 6 | 3 | Machine (id), Money |
| ProductionLine | 5 | 6 | Machine (id) |
| WorkOrder | 7 | 8 | Product, Money, OrderStatus |
| ProductionPlan | 4 | 5 | WorkOrder (id), Money |
| QualityCheck | 6 | 5 | Product (id) |
| DefectReport | 5 | 4 | Product, DocumentNumber |
| Customer | 6 | 6 | Address, ContactInfo, Money |
| OrderItem | 3 | 3 | Product (id), Money |
| SalesOrder | 6 | 7 | Customer (id), OrderItem, Money |
| Contract | 5 | 4 | Customer (id), DatePeriod, Money, SalesOrder |
| Shipment | 6 | 5 | SalesOrder (id) |
| Invoice | 6 | 5 | SalesOrder (id), Money |
| Payment | 5 | 4 | Invoice (id), Money |
| Factory | 7 | 7 | Address, Department, ProductionLine, Warehouse, Employee, WorkOrder, Money |
| MaintenanceRequest | 6 | 4 | Machine (id), Money |
| Tool | 5 | 5 | — |
| EnergyMeter | 4 | 5 | — |
| SafetyInspection | 5 | 4 | — |
| Notification | 5 | 4 | — |
| AuditLog | 4 | 4 | — |
| Report | 5 | 4 | DatePeriod, Money |

Поведения — это методы предметной области, а не геттеры/сеттеры: перевод
денег, расчёт налога и премии, списание материала, запуск станка, приёмка
партии, контроль качества, оплата счёта, расчёт пени, эскалация заявки и т. п.
