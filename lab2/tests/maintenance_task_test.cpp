#include <catch2/catch_test_macros.hpp>
#include "services/maintenance_task.hpp"
TEST_CASE("Maintenance task") { MaintenanceTask t(101,"repair"); REQUIRE_FALSE(t.is_completed()); REQUIRE(t.concerns_room(101)); REQUIRE_FALSE(t.concerns_room(102)); t.complete(); REQUIRE(t.is_completed()); }
