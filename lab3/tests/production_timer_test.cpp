#include "production/ProductionTimer.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("Production timer advances only active lines and completes orders once")
{
    using namespace std::chrono;
    const ProductionTimer::Clock::time_point start{};
    std::map<int, ProductionLine> lines;
    lines.emplace(1, ProductionLine(1, "First", 60));
    lines.emplace(2, ProductionLine(2, "Second", 60));
    lines.at(1).add_machine(1);
    lines.at(1).add_machine(2);
    lines.at(2).add_machine(3);
    lines.at(2).add_machine(4);
    std::map<int, WorkOrder> orders;
    orders.emplace(500, WorkOrder(500, 100, 300, 1, 30));
    orders.emplace(501, WorkOrder(501, 100, 50, 2, 30));
    for (auto& [id, order] : orders)
        order.confirm();
    orders.at(500).record_progress(120);
    ProductionTimer timer;
    timer.track(500, start);
    timer.track(501, start);

    timer.update(orders, lines, start + minutes(10));
    REQUIRE(orders.at(500).completion_percent() == 40);
    lines.at(1).activate();
    timer.update(orders, lines, start + minutes(10) + seconds(59));
    REQUIRE(orders.at(500).completion_percent() == 40);
    timer.update(orders, lines, start + minutes(11));
    REQUIRE(orders.at(500).completion_percent() == 41);
    REQUIRE(orders.at(501).completion_percent() == 0);

    // A newly created order on an active line starts its own clock.
    orders.emplace(502, WorkOrder(502, 100, 7, 1, 30));
    orders.at(502).confirm();
    timer.track(502, start + minutes(11));
    timer.update(orders, lines, start + minutes(12));
    REQUIRE(orders.at(502).completion_percent() == Catch::Approx(1));
    REQUIRE(orders.at(502).completed_quantity() == 0);

    const auto completed = timer.update(orders, lines, start + minutes(70));
    REQUIRE(completed == std::vector<int>{500});
    REQUIRE(orders.at(500).status() == OrderStatus::Completed);
    REQUIRE(orders.at(500).completed_quantity() == 300);
    REQUIRE(timer.update(orders, lines, start + minutes(70)).empty());
    orders.at(502).cancel();
    timer.update(orders, lines, start + minutes(200));
    REQUIRE(orders.at(502).status() == OrderStatus::Cancelled);
    REQUIRE(orders.at(501).completion_percent() == 0);
}

TEST_CASE("Small orders accumulate fractional units and finish exactly at 100 percent")
{
    WorkOrder order(1, 100, 7, 1, 30);
    order.confirm();
    for (int minute = 1; minute <= 100; ++minute)
    {
        order.advance_minutes(1);
        REQUIRE(order.completion_percent() == Catch::Approx(minute));
        REQUIRE(order.completed_quantity() <= 7);
    }
    REQUIRE(order.is_complete());
    REQUIRE(order.remaining_quantity() == 0);
    REQUIRE(order.status() == OrderStatus::Completed);
    order.advance_minutes(100);
    REQUIRE(order.completion_percent() == 100);
}
