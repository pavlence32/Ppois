#pragma once
#include "core/model.hpp"
class ConferenceBooking
{
    int seats_;
    Money price_;
    bool reserved_;
public:
    ConferenceBooking(int seats, Money price);
    void reserve();
    bool is_reserved() const;
    Money total() const;
};
