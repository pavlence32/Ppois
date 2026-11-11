#include <catch2/catch_test_macros.hpp>
#include "rooms/room_key.hpp"
#include "rooms/standard_room.hpp"
#include "people/guest.hpp"
#include "core/exception.hpp"

TEST_CASE("Room key validates owner, room and dates")
{
    Guest guest(30, "Ann", "F", 1, "BY", "Minsk", "Main", 1, 1);
    StandardRoom room(101, nullptr, Money(100), RoomStatus::Free, 2, {}, 20, false);
    StandardRoom other(102, nullptr, Money(100), RoomStatus::Free, 2, {}, 20, false);
    auto from = now_date() - std::chrono::hours(1);
    auto until = now_date() + std::chrono::hours(24);
    RoomKey key("key", &room, &guest, from, until);
    REQUIRE(key.is_valid(now_date()));
    REQUIRE(key.belongs_to(&guest));
    REQUIRE(key.opens_room(&room));
    REQUIRE(key.remaining_days(now_date()) >= 0);
    REQUIRE_THROWS_AS(key.open(&other), InvalidKeyException);
    key.extend(until + std::chrono::hours(24));
    key.deactivate();
    REQUIRE_FALSE(key.is_valid(now_date()));
    REQUIRE_THROWS_AS(key.open(&room), KeyExpiredException);
    REQUIRE_THROWS_AS(RoomKey("bad", &room, &guest, until, from), InvalidDatesException);
}
