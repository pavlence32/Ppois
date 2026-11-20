#pragma once

#include "core/Address.hpp"
#include "core/ContactInfo.hpp"
#include "core/Money.hpp"

#include <string>

class Customer
{
    int id_;
    std::string name_;
    Address address_;
    ContactInfo contact_;
    Money credit_limit_;
    Money balance_;

public:
    Customer(int id,
             std::string name,
             Address address,
             ContactInfo contact,
             Money credit_limit);

    bool can_order(const Money& total) const;
    void charge(const Money& amount);
    void pay(const Money& amount);
    void increase_credit(double amount);
    bool is_over_limit() const;
    Money available_credit() const;
    const std::string& name() const;
};
