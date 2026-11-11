#include <catch2/catch_test_macros.hpp>
#include "core/exception.hpp"
TEST_CASE("EmployeeUnavailableException stores message") { EmployeeUnavailableException e("x"); REQUIRE(e.getMessage() == "x"); }
