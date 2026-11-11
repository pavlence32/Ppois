#include "core/model.hpp"

Money::Money(double amount)
    : amount_(amount)
{}

double Money::value() const
{
    return amount_;
}

Money Money::operator*(double multiplier) const
{
    return Money(amount_ * multiplier);
}

Money Money::operator+(const Money& other) const
{
    return Money(amount_ + other.amount_);
}

Money Money::operator-(const Money& other) const
{
    return Money(amount_ - other.amount_);
}

Money Money::percent(double value) const
{
    return Money(amount_ * value / 100.0);
}

Money Money::add_tax(double rate) const
{
    return *this + percent(rate);
}

Money Money::apply_discount(double rate) const
{
    return *this - percent(rate);
}

bool Money::is_zero() const
{
    return amount_ == 0.0;
}

bool Money::is_positive() const
{
    return amount_ > 0.0;
}

bool valid_date_range(Date start, Date end)
{
    return start < end;
}

Date now_date()
{
    return std::chrono::system_clock::now();
}

int days_between(Date start, Date end)
{
    return static_cast<int>(
        std::chrono::duration_cast<std::chrono::hours>(end - start).count() / 24);
}

bool date_has_passed(Date date, Date now)
{
    return date < now;
}
