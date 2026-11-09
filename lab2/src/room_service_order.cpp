#include "room_service_order.hpp"
RoomServiceOrder::RoomServiceOrder(Money price) : price_(price), delivered_(false) {}
void RoomServiceOrder::deliver() { delivered_ = true; }
bool RoomServiceOrder::is_delivered() const { return delivered_; }
Money RoomServiceOrder::price() const { return price_; }
