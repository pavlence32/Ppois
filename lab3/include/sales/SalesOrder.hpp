#pragma once

#include "core/Money.hpp"
#include "core/enums.hpp"

#include <string>
#include <vector>

class OrderItem
{
    int product_id_;
    int quantity_;
    Money unit_price_;

public:
    OrderItem(int product_id, int quantity, Money unit_price);

    Money total() const;
    Money discounted_total(double percent) const;
    bool is_valid() const;
};

class SalesOrder
{
    int id_;
    int customer_id_;
    std::vector<OrderItem> items_;
    OrderStatus status_;
    int created_day_;
    Money paid_;

public:
    SalesOrder(int id, int customer_id, int created_day);

    Money total() const;
    void add_item(const OrderItem& item);
    void confirm();
    void cancel();
    void register_payment(const Money& amount);
    Money outstanding() const;
    bool is_paid() const;
    int item_count() const;
    int customer_id() const;
    OrderStatus status() const;
};
