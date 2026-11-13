#include "services/breakfast_order.hpp"

BreakfastOrder::BreakfastOrder(int guests, Money price_per_guest)
    : guests_(guests), price_per_guest_(price_per_guest), delivered_(false)
{}

void BreakfastOrder::deliver() { delivered_ = true; }
Money BreakfastOrder::total() const { return price_per_guest_ * guests_; }
bool BreakfastOrder::is_delivered() const { return delivered_; }
