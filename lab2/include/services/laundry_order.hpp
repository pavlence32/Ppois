#pragma once
#include "core/model.hpp"
class LaundryOrder
{
    int items_;
    Money price_per_item_;
    bool ready_;
public:
    LaundryOrder(int items, Money price_per_item);
    void mark_ready();
    bool is_ready() const;
    Money total() const;
};
