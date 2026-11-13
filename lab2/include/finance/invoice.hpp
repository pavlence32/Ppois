#pragma once

#include "core/model.hpp"
#include <string>

class Invoice
{
    std::string number_;
    Money total_;
    bool paid_;

public:
    Invoice(std::string number, Money total);
    void mark_paid();
    bool is_paid() const;
    bool has_number() const;
    Money total() const;
};
