#pragma once
#include "model.hpp"
class SpaBooking
{
    std::string procedure_;
    Money price_;
    bool completed_;
public:
    SpaBooking(std::string procedure, Money price);
    void complete();
    bool is_completed() const;
    Money price() const;
};
