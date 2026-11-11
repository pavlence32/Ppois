#include <catch2/catch_test_macros.hpp>
#include "rooms/Bed.hpp"
TEST_CASE("Bed capacity and type") { Bed single(TypeBed::Single,1), dbl(TypeBed::Double,2); REQUIRE(single.fits_guests(1)); REQUIRE_FALSE(single.fits_guests(2)); REQUIRE(dbl.is_double()); REQUIRE(dbl.get_capacity()==2); }
