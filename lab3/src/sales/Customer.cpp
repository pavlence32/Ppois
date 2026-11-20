#include "sales/Customer.hpp"

#include "core/exceptions.hpp"

Customer::Customer(int id,
                   std::string name,
                   Address address,
                   ContactInfo contact,
                   Money credit_limit)
    : id_(id),
      name_(std::move(name)),
      address_(std::move(address)),
      contact_(std::move(contact)),
      credit_limit_(credit_limit),
      balance_(0.0)
{
    if (!credit_limit_.is_positive())
        throw InvalidMoneyException("Кредитный лимит должен быть положительным");
}

bool Customer::can_order(const Money& total) const
{
    return total.is_positive() && available_credit().can_cover(total);
}

void Customer::charge(const Money& amount)
{
    if (!can_order(amount))
        throw CapacityExceededException("Превышен кредитный лимит клиента " + name_);
    balance_ += amount;
}

void Customer::pay(const Money& amount)
{
    if (!amount.is_positive())
        throw InvalidMoneyException("Платёж должен быть положительным");
    if (amount.can_cover(balance_))
        balance_ = balance_ - amount;
    else
        balance_ = Money(0.0);
}

void Customer::increase_credit(double amount)
{
    if (amount <= 0.0)
        throw InvalidMoneyException("Увеличение лимита должно быть положительным");
    credit_limit_ += Money(amount);
}

bool Customer::is_over_limit() const
{
    return balance_.value() > credit_limit_.value();
}

Money Customer::available_credit() const
{
    return credit_limit_ - balance_;
}

const std::string& Customer::name() const
{
    return name_;
}
