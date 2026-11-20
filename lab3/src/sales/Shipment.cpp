#include "sales/Shipment.hpp"

#include "core/exceptions.hpp"

Shipment::Shipment(int id, int order_id, std::string destination)
    : id_(id),
      order_id_(order_id),
      destination_(std::move(destination)),
      status_(ShipmentStatus::Prepared),
      dispatch_day_(-1),
      delivered_day_(-1)
{
}

void Shipment::dispatch(int day)
{
    if (status_ != ShipmentStatus::Prepared)
        throw InvalidOrderStateException("Отгрузить можно только подготовленную поставку");
    if (day < 0)
        throw InvalidDateRangeException("Дата отгрузки не может быть отрицательной");
    dispatch_day_ = day;
    status_ = ShipmentStatus::InTransit;
}

void Shipment::deliver(int day)
{
    if (status_ != ShipmentStatus::InTransit)
        throw InvalidOrderStateException("Доставить можно только поставку в пути");
    if (day < dispatch_day_)
        throw InvalidDateRangeException("Доставка раньше отгрузки");
    delivered_day_ = day;
    status_ = ShipmentStatus::Delivered;
}

bool Shipment::is_late(int expected_transit_days) const
{
    if (status_ != ShipmentStatus::Delivered)
        return false;
    return transit_days() > expected_transit_days;
}

int Shipment::transit_days() const
{
    if (dispatch_day_ < 0 || delivered_day_ < 0)
        return 0;
    return delivered_day_ - dispatch_day_;
}

void Shipment::mark_lost()
{
    if (status_ == ShipmentStatus::Delivered)
        throw InvalidOrderStateException("Доставленную поставку нельзя признать потерянной");
    status_ = ShipmentStatus::Lost;
}

bool Shipment::is_delivered() const
{
    return status_ == ShipmentStatus::Delivered;
}

ShipmentStatus Shipment::status() const
{
    return status_;
}
