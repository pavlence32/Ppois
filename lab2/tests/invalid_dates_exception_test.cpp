#include <catch2/catch_test_macros.hpp>
#include "core/exception.hpp"
TEST_CASE("InvalidDatesException stores message") { InvalidDatesException e("x"); REQUIRE(e.getMessage() == "x"); }
