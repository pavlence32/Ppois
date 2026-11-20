#pragma once

#include "core/Money.hpp"

class AttendanceRecord
{
    int employee_id_;
    int day_;
    int hours_worked_;
    bool absent_;

public:
    AttendanceRecord(int employee_id, int day, int hours_worked = 0, bool absent = false);

    void mark_present(int hours);
    void mark_absent();
    bool is_overtime(int norm) const;
    int overtime_hours(int norm) const;
    bool is_valid() const;
};

class PayrollEntry
{
    int employee_id_;
    Money gross_;
    Money deductions_;
    int period_days_;
    bool paid_;

public:
    PayrollEntry(int employee_id, Money gross, int period_days);

    Money net() const;
    Money apply_deduction(const Money& amount);
    void mark_paid();
    bool is_consistent() const;
    Money daily_rate() const;
};
