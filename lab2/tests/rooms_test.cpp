#include <catch2/catch_test_macros.hpp>
#include "rooms/amenity.hpp"
TEST_CASE("Amenity lifecycle and price") { Amenity a("wifi",AmenityCategory::Technology,true,Money(5)); REQUIRE(a.get_name()=="wifi"); REQUIRE(a.is_paid()); REQUIRE(a.get_cost(3).value()==15); REQUIRE(a.is_available_for(3)); a.mark_broken(); REQUIRE_FALSE(a.is_usable()); REQUIRE_FALSE(a.is_available_for(3)); a.repair(); a.change_price(Money(7)); REQUIRE(a.get_cost(2).value()==14); }
#include <catch2/catch_test_macros.hpp>
#include "rooms/Bed.hpp"
TEST_CASE("Bed capacity and type") { Bed single(TypeBed::Single,1), dbl(TypeBed::Double,2); REQUIRE(single.fits_guests(1)); REQUIRE_FALSE(single.fits_guests(2)); REQUIRE(dbl.is_double()); REQUIRE(dbl.get_capacity()==2); }
#include <catch2/catch_test_macros.hpp>
#include "rooms/deluxe_room.hpp"
#include "core/exception.hpp"

TEST_CASE("Deluxe room views and pricing")
{
    DeluxeRoom sea(201, nullptr, Money(100), RoomStatus::Free, 2, {}, 25, true, ViewType::Sea);
    DeluxeRoom city(202, nullptr, Money(100), RoomStatus::Free, 2, {}, 25, false, ViewType::City);
    DeluxeRoom park(203, nullptr, Money(100), RoomStatus::Free, 2, {}, 25, false, ViewType::Park);
    REQUIRE(sea.has_balcony());
    REQUIRE(sea.get_view() == ViewType::Sea);
    REQUIRE(sea.has_premium_view());
    REQUIRE(city.get_view() == ViewType::City);
    REQUIRE_FALSE(city.has_premium_view());
    REQUIRE(sea.calculate_price(1).value() == 200);
    REQUIRE(park.calculate_price(1).value() == 170);
    REQUIRE(sea.price_for_stay(2, 10).value() == 315);
    REQUIRE_THROWS_AS(city.calculate_price(0), InvalidDatesException);
}
#include <catch2/catch_test_macros.hpp>
#include "core/exception.hpp"
TEST_CASE("DuplicateRoomException stores message") { DuplicateRoomException e("x"); REQUIRE(e.getMessage() == "x"); }
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
#include <catch2/catch_test_macros.hpp>
#include "rooms/key_card.hpp"
TEST_CASE("Key card validity") { auto n=now_date(); KeyCard c(101,n+std::chrono::hours(24)); REQUIRE(c.is_active()); REQUIRE(c.is_valid(n)); c.deactivate(); REQUIRE_FALSE(c.is_active()); REQUIRE_FALSE(c.is_valid(n)); }
#include <catch2/catch_test_macros.hpp>
#include "core/exception.hpp"
TEST_CASE("KeyExpiredException stores message") { KeyExpiredException e("x"); REQUIRE(e.getMessage() == "x"); }
#include <catch2/catch_test_macros.hpp>
#include "core/exception.hpp"
TEST_CASE("InvalidKeyException stores message") { InvalidKeyException e("x"); REQUIRE(e.getMessage() == "x"); }
#include <catch2/catch_test_macros.hpp>
#include "rooms/room_inspection.hpp"
TEST_CASE("Room inspection") { RoomInspection i(101,"Inspector"); REQUIRE_FALSE(i.passed()); REQUIRE(i.concerns_room(101)); REQUIRE_FALSE(i.concerns_room(102)); i.pass(); REQUIRE(i.passed()); }
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
#include <catch2/catch_test_macros.hpp>
#include "core/exception.hpp"
TEST_CASE("RoomNotAvailableException stores message") { RoomNotAvailableException e("x"); REQUIRE(e.getMessage() == "x"); }
#include <catch2/catch_test_macros.hpp>
#include "core/exception.hpp"
TEST_CASE("RoomNotCleanedException stores message") { RoomNotCleanedException e("x"); REQUIRE(e.getMessage() == "x"); }
#include <catch2/catch_test_macros.hpp>
#include "core/exception.hpp"
TEST_CASE("RoomNotFoundException stores message") { RoomNotFoundException e("x"); REQUIRE(e.getMessage() == "x"); }
#include <catch2/catch_test_macros.hpp>
#include "core/exception.hpp"
TEST_CASE("RoomOverCapacityException stores message") { RoomOverCapacityException e("x"); REQUIRE(e.getMessage() == "x"); }
#include <catch2/catch_test_macros.hpp>

#include "core/exception.hpp"
#include "test_support.hpp"

TEST_CASE("Room goes through reservation, stay and cleaning lifecycle", "[room]")
{
    StandardRoom room = support::make_room();
    const Date from = support::today() + std::chrono::hours(24);
    const Date to = from + std::chrono::hours(24);

    CHECK(room.is_available(from, to));
    room.reserve();
    CHECK(room.get_status() == RoomStatus::Reserved);
    room.occupy();
    CHECK(room.get_status() == RoomStatus::Occupied);
    room.release();
    CHECK(room.get_status() == RoomStatus::Dirty);
    room.mark_clean();
    CHECK(room.get_status() == RoomStatus::Free);
}

TEST_CASE("Room accepts amenities and blocks unavailable dates", "[room]")
{
    StandardRoom room = support::make_room();
    room.add_amenity(Amenity("wifi", AmenityCategory::Technology, false));
    CHECK(room.has_amenity("wifi"));
    CHECK_FALSE(room.has_amenity("missing"));

    const Date from = support::today() + std::chrono::hours(24);
    const Date to = from + std::chrono::hours(24);
    room.add_reservation_dates(from, to);
    CHECK_FALSE(room.is_available(from, to));
    room.remove_reservation_dates(from, to);
    CHECK(room.is_available(from, to));
}

TEST_CASE("Room rejects invalid capacity and state transitions", "[room]")
{
    StandardRoom dirty = support::make_room(102, 2, RoomStatus::Dirty);

    CHECK_FALSE(dirty.has_free_capacity(1));
    CHECK_THROWS_AS(dirty.occupy(), RoomNotCleanedException);
    CHECK_THROWS_AS(dirty.reserve(), RoomNotAvailableException);
    CHECK_THROWS_AS(dirty.can_accommodate(-1), RoomOverCapacityException);
    CHECK_THROWS_AS(dirty.can_accommodate(3), RoomOverCapacityException);
    CHECK_THROWS_AS(dirty.add_reservation_dates(support::today() + std::chrono::hours(24),
                                                support::today()),
                    InvalidDatesException);
}
#include <catch2/catch_test_macros.hpp>
#include "rooms/Roomtype.hpp"
TEST_CASE("Room type") { RoomType t("suite",2.0f); REQUIRE(t.get_type()=="suite"); REQUIRE(t.get_priceMultiplier()==2.0f); }
#include <catch2/catch_test_macros.hpp>
#include "rooms/standard_room.hpp"
#include "core/exception.hpp"

TEST_CASE("Standard room pricing")
{
    StandardRoom room(101, nullptr, Money(100), RoomStatus::Free, 2, {}, 20, true);
    REQUIRE(room.has_balcony());
    REQUIRE_FALSE(room.is_quiet_room());
    REQUIRE(room.calculate_price(1).value() == 100);
    REQUIRE(room.calculate_price(7).value() == 630);
    REQUIRE(room.price_for_stay(2, 10).value() == 180);
    REQUIRE_THROWS_AS(room.calculate_price(0), InvalidDatesException);
}
#include <catch2/catch_test_macros.hpp>
#include "rooms/suite.hpp"
#include "core/exception.hpp"

TEST_CASE("Suite amenities and pricing")
{
    Service included("breakfast", Money(20));
    Suite suite(301, nullptr, Money(100), RoomStatus::Free, 4, {}, 40, 2, true, true, {&included});
    REQUIRE(suite.get_bedrooms() == 2);
    REQUIRE(suite.has_kitchen());
    REQUIRE(suite.has_jacuzzi());
    REQUIRE(suite.includes_service("breakfast"));
    REQUIRE_FALSE(suite.includes_service("spa"));
    REQUIRE(suite.calculate_price(2).value() == 800);
    REQUIRE(suite.price_for_stay(1, 25).value() == 300);
    REQUIRE_THROWS_AS(suite.calculate_price(0), InvalidDatesException);
}
