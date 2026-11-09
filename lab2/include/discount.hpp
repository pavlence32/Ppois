#pragma once

#include "model.hpp"
#include <string>

class Discount
{
    std::string name_;
    double percent_;
    Date valid_until_;

public:
    Discount(std::string name, double percent, Date valid_until);
    Money apply(Money price) const;
    bool is_valid(Date date) const;
    bool is_full() const;
};
