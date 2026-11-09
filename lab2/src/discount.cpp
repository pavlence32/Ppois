#include "discount.hpp"

Discount::Discount(std::string name, double percent, Date valid_until)
    : name_(name), percent_(percent), valid_until_(valid_until)
{}

Money Discount::apply(Money price) const { return price.apply_discount(percent_); }
bool Discount::is_valid(Date date) const { return date < valid_until_; }
bool Discount::is_full() const { return percent_ >= 100.0; }
