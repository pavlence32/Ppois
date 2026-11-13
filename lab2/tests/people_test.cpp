#include <catch2/catch_test_macros.hpp>
#include "people/people.hpp"
TEST_CASE("People stores identity") { People a(18,"Ann","F",1), b(18,"Ann","F",1), c(17,"Bob","M",2); REQUIRE(a.getAge()==18); REQUIRE(a.getName()=="Ann"); REQUIRE(a.getMale()=="F"); REQUIRE(a.getNumber()==1); REQUIRE(a.is_adult()); REQUIRE(a.has_valid_name()); REQUIRE(a.same_person(b)); REQUIRE_FALSE(a.same_person(c)); REQUIRE_FALSE(People(10,"","",0).is_adult()); REQUIRE_FALSE(People(10,"","",0).has_valid_name()); }
#include <catch2/catch_test_macros.hpp>
#include "people/chef.hpp"
TEST_CASE("Chef status") { Chef c(30,"Chef","M",1,"cook"); REQUIRE_FALSE(c.get_status()); c.set_status(true); REQUIRE(c.get_status()); c.work(); }
#include <catch2/catch_test_macros.hpp>
#include "people/complaint.hpp"
TEST_CASE("Complaint") { Complaint urgent("fire",8), normal("late",2); REQUIRE(urgent.is_urgent()); REQUIRE_FALSE(normal.is_urgent()); REQUIRE(urgent.is_open()); urgent.resolve(); REQUIRE_FALSE(urgent.is_open()); }
#include <catch2/catch_test_macros.hpp>
#include "people/employee.hpp"
TEST_CASE("Employee can work") { employee e(30,"Worker","M",1,"desk"); REQUIRE(e.getName()=="Worker"); e.work(); }
#include <catch2/catch_test_macros.hpp>
#include "core/exception.hpp"
TEST_CASE("EmployeeUnavailableException stores message") { EmployeeUnavailableException e("x"); REQUIRE(e.getMessage() == "x"); }
#include <catch2/catch_test_macros.hpp>
#include "people/guest_badge.hpp"
TEST_CASE("Guest badge lifecycle") { GuestBadge b("Ann"); REQUIRE(b.is_active()); REQUIRE(b.belongs_to("Ann")); REQUIRE_FALSE(b.belongs_to("Bob")); b.deactivate(); REQUIRE_FALSE(b.is_active()); }
#include <catch2/catch_test_macros.hpp>
#include "people/guest_feedback.hpp"
TEST_CASE("Guest feedback") { GuestFeedback good("great",4), bad("bad",3); REQUIRE(good.is_positive()); REQUIRE_FALSE(bad.is_positive()); REQUIRE_FALSE(good.is_processed()); good.process(); REQUIRE(good.is_processed()); }
#include <catch2/catch_test_macros.hpp>
#include "people/guest.hpp"
TEST_CASE("Guest address and reservation eligibility") { Guest g(25,"Ann","F",1,"BY","Minsk","Main",10,2); REQUIRE(g.getCountry()=="BY"); REQUIRE(g.getCity()=="Minsk"); REQUIRE(g.getStreet()=="Main"); REQUIRE(g.getHouse()==10); REQUIRE(g.getFlat()==2); REQUIRE(g.has_complete_address()); REQUIRE(g.full_address()=="Minsk, Main 10, 2"); REQUIRE(g.can_make_reservation()); Guest minor(17,"Kid","M",2,"BY","Minsk","Main",1,0); REQUIRE_FALSE(minor.can_make_reservation()); }
#include <catch2/catch_test_macros.hpp>
#include "people/housekeeper.hpp"
TEST_CASE("Housekeeper status") { Housekeeper h(30,"Housekeeper","F",1,"clean"); REQUIRE_FALSE(h.get_status()); h.set_status(true); REQUIRE(h.get_status()); h.work(); }
#include <catch2/catch_test_macros.hpp>
#include "people/manager.hpp"
TEST_CASE("Manager assigns employees") { HotelManager m; Chef c(30,"C","M",1,"cook"); Housekeeper h(30,"H","F",2,"clean"); receptionist r(30,"R","F",3,"front"); r.set_status(false); m.addChef(c); m.addHousekeeper(h); m.addReceptionist(r); m.assignChef(); m.assignHousekeeper(101); m.assignReceptionist(); }
#include <catch2/catch_test_macros.hpp>
#include "people/receptionist.hpp"
TEST_CASE("Receptionist status") { receptionist r(30,"Reception","F",1,"front"); REQUIRE(r.get_status()); r.set_status(false); REQUIRE_FALSE(r.get_status()); r.work(); }
#include <catch2/catch_test_macros.hpp>
#include "people/review.hpp"
TEST_CASE("Review") { Review r(5,"good"), bad(3,"ok"); REQUIRE(r.is_positive()); REQUIRE_FALSE(bad.is_positive()); REQUIRE_FALSE(r.is_published()); r.publish(); REQUIRE(r.is_published()); }
