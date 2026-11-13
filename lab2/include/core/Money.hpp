#pragma once

class Money
{
    double amount_;

public:
    explicit Money(double amount = 0.0);
    double value() const;
    Money operator*(double multiplier) const;
    Money operator+(const Money& other) const;
    Money operator-(const Money& other) const;
    Money percent(double value) const;
    Money add_tax(double rate) const;
    Money apply_discount(double rate) const;
    bool is_zero() const;
    bool is_positive() const;
};
