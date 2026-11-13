#include <catch2/catch_test_macros.hpp>
#include "hotel/conference_booking.hpp"
TEST_CASE("Conference booking") { ConferenceBooking b(10,Money(100)); REQUIRE(b.total().value()==1000); REQUIRE_FALSE(b.is_reserved()); b.reserve(); REQUIRE(b.is_reserved()); }
#include <catch2/catch_test_macros.hpp>
#include "hotel/hotel_event.hpp"
TEST_CASE("Hotel event") { auto n=now_date(); HotelEvent e("concert",n,10), empty("",n,0); REQUIRE(e.has_name()); REQUIRE_FALSE(empty.has_name()); REQUIRE(e.has_started(n)); REQUIRE(e.can_accept(10)); REQUIRE_FALSE(e.can_accept(11)); }
#include <catch2/catch_test_macros.hpp>
#include "hotel/hotel.hpp"
#include "rooms/floor.hpp"
#include "rooms/standard_room.hpp"
#include "people/employee.hpp"
#include "core/exception.hpp"

TEST_CASE("Hotel aggregates floors, rooms and employees")
{
    Hotel hotel("H", 3, {"M", "S", "1"});
    REQUIRE(hotel.get_occupancy_rate() == 0);
    hotel.add_floor(nullptr);
    hotel.hire_employee(nullptr);
    Floor floor(1, &hotel);
    StandardRoom free_room(101, &floor, Money(100), RoomStatus::Free, 2, {}, 20, false);
    StandardRoom dirty_room(102, &floor, Money(100), RoomStatus::Dirty, 2, {}, 20, false);
    floor.add_room(&free_room);
    floor.add_room(&dirty_room);
    hotel.add_floor(&floor);
    employee worker(30, "W", "M", 1, "desk");
    hotel.hire_employee(&worker);
    REQUIRE(hotel.room_count() == 2);
    REQUIRE(hotel.employee_count() == 1);
    REQUIRE(hotel.get_check_out_time() == "12:00");
    REQUIRE(&hotel.find_room(101) == &free_room);
    REQUIRE_THROWS_AS(hotel.find_room(999), RoomNotFoundException);
    REQUIRE(hotel.find_available_rooms(now_date(), now_date() + std::chrono::hours(24), 2).size() == 1);
    REQUIRE(hotel.has_available_room(now_date(), now_date() + std::chrono::hours(24), 2));
    hotel.clean_all_dirty_rooms();
    REQUIRE(dirty_room.get_status() == RoomStatus::Free);
}
#include <catch2/catch_test_macros.hpp>
#include "hotel/house_rule.hpp"
TEST_CASE("House rule") { HouseRule r("No smoking","description"); REQUIRE(r.is_active()); REQUIRE(r.applies_to("No smoking")); REQUIRE_FALSE(r.applies_to("Pets")); r.disable(); REQUIRE_FALSE(r.is_active()); }
#include <catch2/catch_test_macros.hpp>
#include "hotel/parking_ticket.hpp"
TEST_CASE("Parking ticket") { ParkingTicket t("1","ABC"); REQUIRE(t.is_active()); REQUIRE(t.belongs_to_car("ABC")); REQUIRE_FALSE(t.belongs_to_car("XYZ")); t.close(); REQUIRE_FALSE(t.is_active()); }
