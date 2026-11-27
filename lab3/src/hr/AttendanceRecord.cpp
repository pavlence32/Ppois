#include "hr/AttendanceRecord.hpp"

#include "core/exceptions.hpp"

#include <algorithm>

AttendanceRecord::AttendanceRecord(int employee_id, int day, int hours_worked, bool absent)
    : employee_id_(employee_id), day_(day), hours_worked_(hours_worked), absent_(absent)
{
}

void AttendanceRecord::mark_present(int hours)
{
    if (hours < 0 || hours > 16)
        throw FactoryException("Недопустимое количество рабочих часов");
    hours_worked_ = hours;
    absent_ = false;
}

void AttendanceRecord::mark_absent()
{
    hours_worked_ = 0;
    absent_ = true;
}

bool AttendanceRecord::is_overtime(int norm) const
{
    return !absent_ && hours_worked_ > norm;
}

int AttendanceRecord::overtime_hours(int norm) const
{
    return std::max(0, hours_worked_ - norm);
}

bool AttendanceRecord::is_valid() const
{
    return employee_id_ > 0 && day_ >= 0 && (absent_ || hours_worked_ > 0);
}

PayrollEntry::PayrollEntry(int employee_id, Money gross, int period_days)
    : employee_id_(employee_id), gross_(gross), period_days_(period_days), paid_(false)
{
}

Money PayrollEntry::net() const
{
    return gross_ - deductions_;
}

Money PayrollEntry::apply_deduction(const Money& amount)
{
    if (!gross_.can_cover(deductions_ + amount))
        throw InvalidMoneyException("Удержание превышает начисление");
    deductions_ += amount;
    return deductions_;
}

void PayrollEntry::mark_paid()
{
    if (!is_consistent())
        throw InvalidOrderStateException("Нельзя выплатить некорректную ведомость");
    paid_ = true;
}

bool PayrollEntry::is_consistent() const
{
    return employee_id_ > 0 && period_days_ > 0 && gross_.is_positive()
        && !deductions_.can_cover(gross_);
}

Money PayrollEntry::daily_rate() const
{
    return gross_ * (1.0 / period_days_);
}
