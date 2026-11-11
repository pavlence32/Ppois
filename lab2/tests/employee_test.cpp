#include <catch2/catch_test_macros.hpp>
#include "people/employee.hpp"
TEST_CASE("Employee can work") { employee e(30,"Worker","M",1,"desk"); REQUIRE(e.getName()=="Worker"); e.work(); }
