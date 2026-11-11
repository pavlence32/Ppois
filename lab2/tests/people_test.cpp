#include <catch2/catch_test_macros.hpp>
#include "people/people.hpp"
TEST_CASE("People stores identity") { People a(18,"Ann","F",1), b(18,"Ann","F",1), c(17,"Bob","M",2); REQUIRE(a.getAge()==18); REQUIRE(a.getName()=="Ann"); REQUIRE(a.getMale()=="F"); REQUIRE(a.getNumber()==1); REQUIRE(a.is_adult()); REQUIRE(a.has_valid_name()); REQUIRE(a.same_person(b)); REQUIRE_FALSE(a.same_person(c)); REQUIRE_FALSE(People(10,"","",0).is_adult()); REQUIRE_FALSE(People(10,"","",0).has_valid_name()); }
