#pragma once

#include "model.hpp"
#include <string>

class Payment
{
    Money amount_;
    std::string method_;
    bool completed_;

public:
    Payment(Money amount, std::string method);
    void complete();
    bool is_completed() const;
    bool is_card_payment() const;
    Money amount() const;
};
