#include "production/WorkOrder.hpp"

#include "core/exceptions.hpp"

#include <algorithm>

WorkOrder::WorkOrder(int id,
                     int product_id,
                     int quantity,
                     int line_id,
                     int due_day,
                     OrderStatus status)
    : id_(id),
      product_id_(product_id),
      quantity_(quantity),
      line_id_(line_id),
      status_(status),
      due_day_(due_day),
      completed_quantity_(0)
{
    if (quantity <= 0)
        throw FactoryException("Заказ должен содержать хотя бы одно изделие");
}

bool WorkOrder::is_complete() const
{
    return completed_quantity_ >= quantity_;
}

void WorkOrder::record_progress(int units)
{
    if (status_ != OrderStatus::Confirmed && status_ != OrderStatus::InProduction)
        throw InvalidOrderStateException("Прогресс можно фиксировать только для запущенного заказа");
    if (units <= 0)
        throw FactoryException("Прогресс должен быть положительным");
    completed_quantity_ = std::min(quantity_, completed_quantity_ + units);
    if (is_complete())
        partial_quantity_ = 0.0;
    status_ = is_complete() ? OrderStatus::Completed : OrderStatus::InProduction;
}

double WorkOrder::completion_percent() const
{
    return 100.0 * (completed_quantity_ + partial_quantity_) / quantity_;
}

bool WorkOrder::is_overdue(int today) const
{
    return !is_complete() && today > due_day_;
}

void WorkOrder::cancel()
{
    if (status_ == OrderStatus::Completed)
        throw InvalidOrderStateException("Завершённый заказ нельзя отменить");
    if (status_ == OrderStatus::Cancelled)
        throw InvalidOrderStateException("Заказ уже отменён");
    status_ = OrderStatus::Cancelled;
}

void WorkOrder::confirm()
{
    if (status_ != OrderStatus::Draft)
        throw InvalidOrderStateException("Подтвердить можно только черновик заказа");
    status_ = OrderStatus::Confirmed;
}

Money WorkOrder::estimated_cost(const Product& product) const
{
    return product.unit_price() * static_cast<double>(quantity_);
}

int WorkOrder::remaining_quantity() const
{
    return quantity_ - completed_quantity_;
}

OrderStatus WorkOrder::status() const
{
    return status_;
}

int WorkOrder::line_id() const
{
    return line_id_;
}

void WorkOrder::advance_minutes(int minutes)
{
    if (minutes <= 0 || (status_ != OrderStatus::Confirmed && status_ != OrderStatus::InProduction))
        return;
    const double total = std::min(static_cast<double>(quantity_),
                                 completed_quantity_ + partial_quantity_ + quantity_ * (minutes / 100.0));
    completed_quantity_ = static_cast<int>(total + 1e-9);
    partial_quantity_ = std::max(0.0, total - completed_quantity_);
    status_ = is_complete() ? OrderStatus::Completed : OrderStatus::InProduction;
}

ProductionPlan::ProductionPlan(std::string period, Money budget, double utilization_target)
    : period_(std::move(period)),
      budget_(budget),
      utilization_target_(utilization_target)
{
}

void ProductionPlan::add_order(int order_id)
{
    if (order_id <= 0)
        throw EntityNotFoundException("Заказ не найден");
    if (std::find(order_ids_.begin(), order_ids_.end(), order_id) != order_ids_.end())
        throw DuplicateEntityException("Заказ уже включён в план");
    order_ids_.push_back(order_id);
}

bool ProductionPlan::remove_order(int order_id)
{
    const auto it = std::find(order_ids_.begin(), order_ids_.end(), order_id);
    if (it == order_ids_.end())
        return false;
    order_ids_.erase(it);
    return true;
}

bool ProductionPlan::is_within_budget(const Money& planned) const
{
    return budget_.can_cover(planned);
}

double ProductionPlan::load_percent(int capacity) const
{
    if (capacity <= 0)
        throw CapacityExceededException("Производственная мощность должна быть положительной");
    return 100.0 * static_cast<double>(order_ids_.size()) / capacity;
}

Money ProductionPlan::average_order_budget() const
{
    if (order_ids_.empty())
        return Money(0.0);
    return budget_ * (1.0 / static_cast<double>(order_ids_.size()));
}

int ProductionPlan::order_count() const
{
    return static_cast<int>(order_ids_.size());
}
int WorkOrder::completed_quantity() const
{
    return completed_quantity_;
}

int WorkOrder::product_id() const
{
    return product_id_;
}
