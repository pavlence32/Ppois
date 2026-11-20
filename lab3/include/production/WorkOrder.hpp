#pragma once

#include "core/Money.hpp"
#include "core/enums.hpp"
#include "production/Product.hpp"

#include <string>

class WorkOrder
{
    int id_;
    int product_id_;
    int quantity_;
    int line_id_;
    OrderStatus status_;
    int due_day_;
    int completed_quantity_;

public:
    WorkOrder(int id,
              int product_id,
              int quantity,
              int line_id,
              int due_day,
              OrderStatus status = OrderStatus::Draft);

    bool is_complete() const;
    void record_progress(int units);
    double completion_percent() const;
    bool is_overdue(int today) const;
    void cancel();
    void confirm();
    Money estimated_cost(const Product& product) const;
    int remaining_quantity() const;
    OrderStatus status() const;
};

class ProductionPlan
{
    std::string period_;
    std::vector<int> order_ids_;
    Money budget_;
    double utilization_target_;

public:
    ProductionPlan(std::string period, Money budget, double utilization_target);

    void add_order(int order_id);
    bool remove_order(int order_id);
    bool is_within_budget(const Money& planned) const;
    double load_percent(int capacity) const;
    Money average_order_budget() const;
    int order_count() const;
};
