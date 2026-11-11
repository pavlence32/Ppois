#include "services/pool_visit.hpp"
PoolVisit::PoolVisit(int guests, Date visit_date)
    : guests_(guests), visit_date_(visit_date), checked_(false) {}
void PoolVisit::check() { checked_ = true; }
bool PoolVisit::is_checked() const { return checked_; }
bool PoolVisit::is_today(Date date) const
{
    return visit_date_ == date;
}
