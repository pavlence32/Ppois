#pragma once
#include "model.hpp"
class RoomServiceOrder
{
    Money price_;
    bool delivered_;
public:
    explicit RoomServiceOrder(Money price);
    void deliver();
    bool is_delivered() const;
    Money price() const;
};
