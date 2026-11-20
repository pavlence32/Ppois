#pragma once

enum class EmployeeRole
{
    Worker,
    Foreman,
    Engineer,
    Accountant,
    Storekeeper,
    QualityInspector,
    SecurityGuard
};

enum class MachineStatus
{
    Idle,
    Running,
    Broken,
    UnderMaintenance
};

enum class OrderStatus
{
    Draft,
    Confirmed,
    InProduction,
    Completed,
    Cancelled
};

enum class QualityResult
{
    Passed,
    Failed,
    Rework
};

enum class Priority
{
    Low,
    Normal,
    High,
    Critical
};

enum class ShipmentStatus
{
    Prepared,
    InTransit,
    Delivered,
    Lost
};

enum class PaymentMethod
{
    Cash,
    BankTransfer,
    Card,
    Credit
};
