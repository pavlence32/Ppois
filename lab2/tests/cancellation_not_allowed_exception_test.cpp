#include <catch2/catch_test_macros.hpp>
#include "core/exception.hpp"
TEST_CASE("CancellationNotAllowedException stores message") { CancellationNotAllowedException e("x"); REQUIRE(e.getMessage() == "x"); }
