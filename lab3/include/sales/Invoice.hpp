#pragma once

#include "core/Money.hpp"

#include <string>

class Invoice
{
    int id_;
    int order_id_;
    Money amount_;
    Money paid_;
    int due_day_;
    bool issued_;

public:
    Invoice(int id, int order_id, Money amount, int due_day);

    Money balance() const;
    bool is_overdue(int today) const;
    void apply_payment(const Money& amount);
    void issue();
    bool is_settled() const;
    bool is_issued() const;
};

class Payment
{
    int id_;
    int invoice_id_;
    Money amount_;
    std::string method_;
    bool confirmed_;

public:
    Payment(int id, int invoice_id, Money amount, std::string method);

    void confirm();
    void refund();
    bool is_large(double threshold) const;
    std::string receipt() const;
    bool is_confirmed() const;
    const Money& amount() const;
};
