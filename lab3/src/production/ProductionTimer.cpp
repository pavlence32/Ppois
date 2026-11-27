#include "production/ProductionTimer.hpp"

#include <algorithm>

void ProductionTimer::track(int order_id, Clock::time_point now)
{
    ticks_[order_id] = now;
}

std::vector<int> ProductionTimer::update(std::map<int, WorkOrder>& orders,
                                       const std::map<int, ProductionLine>& lines,
                                       Clock::time_point now)
{
    std::vector<int> completed;
    for (auto& [id, order] : orders)
    {
        auto it = ticks_.emplace(id, now).first;
        auto& tick = it->second;
        if (!lines.at(order.line_id()).is_active() || order.is_complete()
            || order.status() == OrderStatus::Cancelled || order.status() == OrderStatus::Draft)
        {
            tick = now;
            continue;
        }
        const auto minutes = std::chrono::duration_cast<std::chrono::minutes>(now - tick).count();
        if (minutes <= 0)
            continue;
        // No order needs more than 100 minutes to finish.
        order.advance_minutes(static_cast<int>(std::min(minutes, decltype(minutes)(100))));
        tick += std::chrono::minutes(minutes);
        if (order.is_complete())
            completed.push_back(id);
    }
    return completed;
}
