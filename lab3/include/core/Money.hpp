#pragma once

class Money
{
    double amount_;

public:
    explicit Money(double amount = 0.0);

    double value() const;
    Money operator+(const Money& other) const;
    Money operator-(const Money& other) const;
    Money operator*(double multiplier) const;
    Money percent(double value) const;
    Money with_tax(double rate) const;
    Money with_discount(double rate) const;
    bool is_zero() const;
    bool is_positive() const;
    bool can_cover(const Money& other) const;
    Money& operator+=(const Money& other);
};
