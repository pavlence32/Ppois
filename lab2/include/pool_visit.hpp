#pragma once
#include "model.hpp"
class PoolVisit
{
    int guests_;
    Date visit_date_;
    bool checked_;
public:
    PoolVisit(int guests, Date visit_date);
    void check();
    bool is_checked() const;
    bool is_today(Date date) const;
};
