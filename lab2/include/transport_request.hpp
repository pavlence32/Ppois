#pragma once
#include "model.hpp"
class TransportRequest
{
    int guests_;
    Money price_;
    bool confirmed_;
public:
    TransportRequest(int guests, Money price);
    void confirm();
    bool is_confirmed() const;
    Money price() const;
};
