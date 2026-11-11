#include <catch2/catch_test_macros.hpp>
#include "core/exception.hpp"
TEST_CASE("InvalidKeyException stores message") { InvalidKeyException e("x"); REQUIRE(e.getMessage() == "x"); }
