#include "sales/SalesOrder.hpp"

#include "core/exceptions.hpp"

OrderItem::OrderItem(int product_id, int quantity, Money unit_price)
    : product_id_(product_id), quantity_(quantity), unit_price_(unit_price)
{
}

Money OrderItem::total() const
{
    return unit_price_ * static_cast<double>(quantity_);
}

Money OrderItem::discounted_total(double percent) const
{
    if (percent < 0.0 || percent > 100.0)
        throw InvalidMoneyException("Скидка должна быть в диапазоне 0..100");
    return total().with_discount(percent);
}

bool OrderItem::is_valid() const
{
    return product_id_ > 0 && quantity_ > 0 && unit_price_.is_positive();
}

SalesOrder::SalesOrder(int id, int customer_id, int created_day)
    : id_(id),
      customer_id_(customer_id),
      status_(OrderStatus::Draft),
      created_day_(created_day),
      paid_(0.0)
{
    if (customer_id <= 0)
        throw EntityNotFoundException("Клиент не найден");
}

Money SalesOrder::total() const
{
    Money result(0.0);
    for (const auto& item : items_)
        result += item.total();
    return result;
}

void SalesOrder::add_item(const OrderItem& item)
{
    if (status_ != OrderStatus::Draft)
        throw InvalidOrderStateException("Позиции можно менять только в черновике");
    if (!item.is_valid())
        throw FactoryException("Некорректная позиция заказа");
    items_.push_back(item);
}

void SalesOrder::confirm()
{
    if (status_ != OrderStatus::Draft)
        throw InvalidOrderStateException("Заказ уже обработан");
    if (items_.empty())
        throw InvalidOrderStateException("Нельзя подтвердить пустой заказ");
    status_ = OrderStatus::Confirmed;
}

void SalesOrder::cancel()
{
    if (status_ == OrderStatus::Completed)
        throw InvalidOrderStateException("Выполненный заказ нельзя отменить");
    status_ = OrderStatus::Cancelled;
}

void SalesOrder::register_payment(const Money& amount)
{
    if (status_ != OrderStatus::Confirmed)
        throw InvalidOrderStateException("Оплата возможна только для подтверждённого заказа");
    if (!amount.is_positive())
        throw InvalidMoneyException("Сумма оплаты должна быть положительной");
    if (amount.value() > outstanding().value())
        throw InvalidMoneyException("Оплата превышает остаток по заказу");
    paid_ += amount;
    if (is_paid())
        status_ = OrderStatus::Completed;
}

Money SalesOrder::outstanding() const
{
    return total() - paid_;
}

bool SalesOrder::is_paid() const
{
    return paid_.value() >= total().value();
}

int SalesOrder::item_count() const
{
    return static_cast<int>(items_.size());
}

int SalesOrder::customer_id() const
{
    return customer_id_;
}

OrderStatus SalesOrder::status() const
{
    return status_;
}
