#include <catch2/catch_test_macros.hpp>
#include "rooms/floor.hpp"
#include "rooms/standard_room.hpp"
#include "core/exception.hpp"

TEST_CASE("Floor stores rooms and counts statuses")
{
    Floor floor(1);
    StandardRoom free_room(101, &floor, Money(100), RoomStatus::Free, 2, {}, 20, false);
    StandardRoom dirty_room(102, &floor, Money(100), RoomStatus::Dirty, 2, {}, 20, false);
    StandardRoom occupied_room(103, &floor, Money(100), RoomStatus::Reserved, 2, {}, 20, false);
    occupied_room.occupy();
    floor.add_room(&free_room);
    floor.add_room(&dirty_room);
    floor.add_room(&occupied_room);
    REQUIRE(floor.get_number() == 1);
    REQUIRE(floor.get_rooms().size() == 3);
    REQUIRE(floor.count_free() == 1);
    REQUIRE(floor.count_dirty() == 1);
    REQUIRE(floor.count_occupied() == 1);
    REQUIRE(floor.get_rooms_to_clean().size() == 1);
    REQUIRE(floor.find_room(101) == &free_room);
    REQUIRE(floor.find_room(999) == nullptr);
    REQUIRE(floor.find_free_rooms(now_date(), now_date() + std::chrono::hours(24)).size() == 1);
    REQUIRE_THROWS_AS(floor.add_room(nullptr), DuplicateRoomException);
    REQUIRE_THROWS_AS(floor.add_room(&free_room), DuplicateRoomException);
}
