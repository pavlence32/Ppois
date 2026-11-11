#include <catch2/catch_test_macros.hpp>
#include "core/exception.hpp"
TEST_CASE("InvalidPaymentException stores message") { InvalidPaymentException e("x"); REQUIRE(e.getMessage() == "x"); }
