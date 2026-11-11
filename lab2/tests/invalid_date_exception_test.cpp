#include <catch2/catch_test_macros.hpp>
#include "core/exception.hpp"
TEST_CASE("InvalidDateException stores message") { InvalidDateException e("x"); REQUIRE(e.getMessage() == "x"); }
