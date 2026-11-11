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
