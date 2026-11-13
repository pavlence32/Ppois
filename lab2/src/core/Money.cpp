#include "core/Money.hpp"

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
