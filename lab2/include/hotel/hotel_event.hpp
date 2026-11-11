#pragma once

#include "core/model.hpp"
#include <string>

class HotelEvent
{
    std::string name_;
    Date starts_at_;
    int capacity_;

public:
    HotelEvent(std::string name, Date starts_at, int capacity);
    bool has_started(Date now) const;
    bool can_accept(int guests) const;
    bool has_name() const;
};
