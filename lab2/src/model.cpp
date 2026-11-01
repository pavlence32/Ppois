#include "model.hpp"

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

bool valid_date_range(Date start, Date end)
{
    return start < end;
}

Date now_date()
{
    return std::chrono::system_clock::now();
}
