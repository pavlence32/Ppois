#include "sales/Contract.hpp"

#include "core/exceptions.hpp"

#include <algorithm>

Contract::Contract(int id, int customer_id, DatePeriod term, Money value)
    : id_(id), customer_id_(customer_id), term_(term), value_(value), signed_(false)
{
    if (!term_.is_valid())
        throw InvalidDateRangeException("Срок договора задан неверно");
}

void Contract::sign()
{
    if (signed_)
        throw InvalidOrderStateException("Договор уже подписан");
    if (!value_.is_positive())
        throw InvalidMoneyException("Стоимость договора должна быть положительной");
    signed_ = true;
}

bool Contract::is_active(int day) const
{
    return signed_ && term_.contains(day);
}

Money Contract::penalty(int delay_days) const
{
    if (delay_days <= 0)
        return Money(0.0);
    const double rate = std::min(20.0, delay_days * 0.5);
    return value_.percent(rate);
}

bool Contract::matches_order(const SalesOrder& order) const
{
    return order.customer_id() == customer_id_ && value_.can_cover(order.total());
}

bool Contract::is_signed() const
{
    return signed_;
}
