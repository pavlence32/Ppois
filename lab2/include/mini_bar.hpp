#pragma once
#include "model.hpp"
class MiniBar
{
    int products_;
    Money total_price_;
    bool sealed_;
public:
    MiniBar(int products, Money total_price);
    void seal();
    bool is_sealed() const;
    Money price() const;
};
