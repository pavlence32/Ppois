#pragma once

#include "model.hpp"

class Service
{
    std::string name_;
    Money price_;
    std::string category_;
    int duration_minutes_ = 0;
    bool available_ = true;
    int daily_limit_ = 0;

public:
    Service(std::string name, Money price = Money{});
    const std::string& get_name() const;
    Money get_price() const;
    Money price_for(int count) const;
    Money price_with_discount(int count, double rate) const;
    bool is_free() const;
};
