#pragma once

#include "model.hpp"

class BreakfastOrder
{
    int guests_;
    Money price_per_guest_;
    bool delivered_;

public:
    BreakfastOrder(int guests, Money price_per_guest);
    void deliver();
    Money total() const;
    bool is_delivered() const;
};
