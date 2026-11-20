#pragma once

#include "core/enums.hpp"

#include <string>

class Shipment
{
    int id_;
    int order_id_;
    std::string destination_;
    ShipmentStatus status_;
    int dispatch_day_;
    int delivered_day_;

public:
    Shipment(int id, int order_id, std::string destination);

    void dispatch(int day);
    void deliver(int day);
    bool is_late(int expected_day) const;
    int transit_days() const;
    void mark_lost();
    bool is_delivered() const;
    ShipmentStatus status() const;
};
