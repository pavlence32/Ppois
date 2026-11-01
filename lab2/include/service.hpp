#pragma once

#include "model.hpp"

class Service
{
    std::string name_;
    Money price_;

public:
    Service(std::string name, Money price = Money{});
    const std::string& get_name() const;
    Money get_price() const;
};
