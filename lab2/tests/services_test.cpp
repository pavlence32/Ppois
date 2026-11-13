#include <catch2/catch_test_macros.hpp>
#include "services/breakfast_order.hpp"
TEST_CASE("Breakfast order") { BreakfastOrder o(2,Money(10)); REQUIRE(o.total().value()==20); REQUIRE_FALSE(o.is_delivered()); o.deliver(); REQUIRE(o.is_delivered()); }
#include <catch2/catch_test_macros.hpp>
#include "services/laundry_order.hpp"
TEST_CASE("Laundry order") { LaundryOrder o(3,Money(4)); REQUIRE(o.total().value()==12); REQUIRE_FALSE(o.is_ready()); o.mark_ready(); REQUIRE(o.is_ready()); }
#include <catch2/catch_test_macros.hpp>
#include "services/maintenance_task.hpp"
TEST_CASE("Maintenance task") { MaintenanceTask t(101,"repair"); REQUIRE_FALSE(t.is_completed()); REQUIRE(t.concerns_room(101)); REQUIRE_FALSE(t.concerns_room(102)); t.complete(); REQUIRE(t.is_completed()); }
#include <catch2/catch_test_macros.hpp>
#include "services/mini_bar.hpp"
TEST_CASE("Mini bar") { MiniBar m(2,Money(20)); REQUIRE(m.price().value()==20); REQUIRE_FALSE(m.is_sealed()); m.seal(); REQUIRE(m.is_sealed()); }
#include <catch2/catch_test_macros.hpp>
#include "services/pool_visit.hpp"
TEST_CASE("Pool visit") { auto n=now_date(); PoolVisit p(2,n); REQUIRE(p.is_today(n)); REQUIRE_FALSE(p.is_checked()); p.check(); REQUIRE(p.is_checked()); }
#include <catch2/catch_test_macros.hpp>
#include "services/room_service_order.hpp"
TEST_CASE("Room service order") { RoomServiceOrder o(Money(25)); REQUIRE(o.price().value()==25); REQUIRE_FALSE(o.is_delivered()); o.deliver(); REQUIRE(o.is_delivered()); }
#include <catch2/catch_test_macros.hpp>
#include "services/service.hpp"
TEST_CASE("Service prices") { Service s("spa",Money(50)), free("free"); REQUIRE(s.get_name()=="spa"); REQUIRE(s.get_price().value()==50); REQUIRE(s.price_for(2).value()==100); REQUIRE(s.price_with_discount(2,10).value()==90); REQUIRE_FALSE(s.is_free()); REQUIRE(free.is_free()); }
#include <catch2/catch_test_macros.hpp>
#include "services/spa_booking.hpp"
TEST_CASE("Spa booking") { SpaBooking b("massage",Money(40)); REQUIRE(b.price().value()==40); REQUIRE_FALSE(b.is_completed()); b.complete(); REQUIRE(b.is_completed()); }
#include <catch2/catch_test_macros.hpp>
#include "services/transport_request.hpp"
TEST_CASE("Transport request") { TransportRequest r(2,Money(30)); REQUIRE(r.price().value()==30); REQUIRE_FALSE(r.is_confirmed()); r.confirm(); REQUIRE(r.is_confirmed()); }
