#include <catch2/catch_test_macros.hpp>
#include "rooms/standard_room.hpp"
#include "core/exception.hpp"

TEST_CASE("Room state and capacity")
{
    StandardRoom room(101, nullptr, Money(100), RoomStatus::Free, 2, {}, 20, false);
    REQUIRE(room.get_number() == 101);
    REQUIRE(room.get_floor() == nullptr);
    REQUIRE(room.get_capacity() == 2);
    REQUIRE(room.get_area() == 20);
    REQUIRE(room.free_beds_count() == 2);
    REQUIRE(room.is_available(now_date(), now_date() + std::chrono::hours(24)));
    REQUIRE(room.has_free_capacity(2));
    REQUIRE_FALSE(room.has_free_capacity(3));
    REQUIRE_THROWS_AS(room.can_accommodate(3), RoomOverCapacityException);
    room.reserve();
    REQUIRE(room.get_status() == RoomStatus::Reserved);
    room.occupy();
    REQUIRE(room.get_status() == RoomStatus::Occupied);
    room.release();
    REQUIRE(room.get_status() == RoomStatus::Dirty);
    room.mark_clean();
    REQUIRE(room.get_status() == RoomStatus::Free);
    REQUIRE_THROWS_AS(room.cancel_reservation(), RoomNotAvailableException);
    REQUIRE_THROWS_AS(room.occupy(), RoomNotAvailableException);
    REQUIRE_THROWS_AS(room.release(), RoomNotAvailableException);
    room.reserve();
    REQUIRE_THROWS_AS(room.reserve(), RoomNotAvailableException);
    room.cancel_reservation();
    room.add_amenity(Amenity("wifi", AmenityCategory::Technology, false));
    REQUIRE(room.has_amenity("wifi"));
    REQUIRE_FALSE(room.has_amenity("missing"));
    room.add_reservation_dates(now_date(), now_date() + std::chrono::hours(24));
    REQUIRE_FALSE(room.is_available(now_date(), now_date() + std::chrono::hours(24)));
    room.remove_reservation_dates(now_date(), now_date() + std::chrono::hours(24));
    REQUIRE(room.price_for_guests(2, 1).value() == 100);
    REQUIRE_THROWS_AS(room.add_reservation_dates(now_date(), now_date()),
                      InvalidDatesException);
    REQUIRE_THROWS_AS(room.price_for_guests(1, 0), InvalidDatesException);
}

TEST_CASE("Room rejects dirty and invalid transitions")
{
    StandardRoom dirty(102, nullptr, Money(100), RoomStatus::Dirty, 2, {}, 20, false);
    REQUIRE_FALSE(dirty.is_available(now_date(), now_date() + std::chrono::hours(24)));
    REQUIRE_FALSE(dirty.has_free_capacity(1));
    REQUIRE_THROWS_AS(dirty.occupy(), RoomNotCleanedException);
    REQUIRE_THROWS_AS(dirty.reserve(), RoomNotAvailableException);
    REQUIRE_THROWS_AS(dirty.can_accommodate(-1), RoomOverCapacityException);
}
