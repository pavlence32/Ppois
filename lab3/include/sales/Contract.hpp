#pragma once

#include "core/DatePeriod.hpp"
#include "core/Money.hpp"
#include "sales/SalesOrder.hpp"

#include <string>

class Contract
{
    int id_;
    int customer_id_;
    DatePeriod term_;
    Money value_;
    bool signed_;

public:
    Contract(int id, int customer_id, DatePeriod term, Money value);

    void sign();
    bool is_active(int day) const;
    Money penalty(int delay_days) const;
    bool matches_order(const SalesOrder& order) const;
    bool is_signed() const;
};
