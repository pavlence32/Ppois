#include "sales/Invoice.hpp"

#include "core/exceptions.hpp"

#include <sstream>

Invoice::Invoice(int id, int order_id, Money amount, int due_day)
    : id_(id), order_id_(order_id), amount_(amount), paid_(0.0), due_day_(due_day), issued_(false)
{
    if (!amount_.is_positive())
        throw InvalidMoneyException("Сумма счёта должна быть положительной");
}

Money Invoice::balance() const
{
    return amount_ - paid_;
}

bool Invoice::is_overdue(int today) const
{
    return issued_ && today > due_day_ && !is_settled();
}

void Invoice::apply_payment(const Money& amount)
{
    if (!issued_)
        throw InvalidOrderStateException("Нельзя оплатить невыставленный счёт");
    if (!amount.is_positive())
        throw InvalidMoneyException("Платёж должен быть положительным");
    if (amount.value() > balance().value())
        throw InvalidMoneyException("Платёж превышает остаток по счёту");
    paid_ += amount;
}

void Invoice::issue()
{
    if (issued_)
        throw InvalidOrderStateException("Счёт уже выставлен");
    issued_ = true;
}

bool Invoice::is_settled() const
{
    return paid_.value() >= amount_.value();
}

bool Invoice::is_issued() const
{
    return issued_;
}

Payment::Payment(int id, int invoice_id, Money amount, std::string method)
    : id_(id),
      invoice_id_(invoice_id),
      amount_(amount),
      method_(std::move(method)),
      confirmed_(false)
{
    if (!amount_.is_positive())
        throw InvalidMoneyException("Сумма платежа должна быть положительной");
}

void Payment::confirm()
{
    if (confirmed_)
        throw InvalidOrderStateException("Платёж уже подтверждён");
    confirmed_ = true;
}

void Payment::refund()
{
    if (!confirmed_)
        throw InvalidOrderStateException("Нельзя вернуть неподтверждённый платёж");
    confirmed_ = false;
}

bool Payment::is_large(double threshold) const
{
    return amount_.value() >= threshold;
}

std::string Payment::receipt() const
{
    std::ostringstream stream;
    stream << "Платёж #" << id_ << " по счёту " << invoice_id_ << " на сумму "
           << amount_.value() << " через " << method_
           << (confirmed_ ? " (подтверждён)" : " (ожидает)");
    return stream.str();
}

bool Payment::is_confirmed() const
{
    return confirmed_;
}

const Money& Payment::amount() const
{
    return amount_;
}
