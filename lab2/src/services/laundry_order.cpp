#include "services/laundry_order.hpp"
LaundryOrder::LaundryOrder(int items, Money price_per_item)
    : items_(items), price_per_item_(price_per_item), ready_(false) {}
void LaundryOrder::mark_ready() { ready_ = true; }
bool LaundryOrder::is_ready() const { return ready_; }
Money LaundryOrder::total() const { return price_per_item_ * items_; }
