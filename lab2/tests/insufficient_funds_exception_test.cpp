#include <catch2/catch_test_macros.hpp>
#include "core/exception.hpp"
TEST_CASE("InsufficientFundsException stores message") { InsufficientFundsException e("x"); REQUIRE(e.getMessage() == "x"); }
