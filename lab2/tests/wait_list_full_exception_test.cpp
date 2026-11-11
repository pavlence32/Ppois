#include <catch2/catch_test_macros.hpp>
#include "core/exception.hpp"
TEST_CASE("WaitListFullException stores message") { WaitListFullException e("x"); REQUIRE(e.getMessage() == "x"); }
