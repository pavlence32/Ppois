#include <catch2/catch_test_macros.hpp>
#include "people/guest.hpp"
TEST_CASE("Guest address and reservation eligibility") { Guest g(25,"Ann","F",1,"BY","Minsk","Main",10,2); REQUIRE(g.getCountry()=="BY"); REQUIRE(g.getCity()=="Minsk"); REQUIRE(g.getStreet()=="Main"); REQUIRE(g.getHouse()==10); REQUIRE(g.getFlat()==2); REQUIRE(g.has_complete_address()); REQUIRE(g.full_address()=="Minsk, Main 10, 2"); REQUIRE(g.can_make_reservation()); Guest minor(17,"Kid","M",2,"BY","Minsk","Main",1,0); REQUIRE_FALSE(minor.can_make_reservation()); }
