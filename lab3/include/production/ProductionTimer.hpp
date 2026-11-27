#pragma once

#include "production/ProductionLine.hpp"
#include "production/WorkOrder.hpp"

#include <chrono>
#include <map>
#include <vector>

class ProductionTimer
{
public:
    using Clock = std::chrono::steady_clock;

    // Register each order when it is created so it cannot inherit earlier runtime.
    void track(int order_id, Clock::time_point now = Clock::now());
    std::vector<int> update(std::map<int, WorkOrder>& orders,
                            const std::map<int, ProductionLine>& lines,
                            Clock::time_point now = Clock::now());

private:
    std::map<int, Clock::time_point> ticks_;
};
