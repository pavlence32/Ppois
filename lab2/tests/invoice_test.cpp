#include <catch2/catch_test_macros.hpp>
#include "finance/invoice.hpp"
TEST_CASE("Invoice") { Invoice i("INV-1",Money(100)); REQUIRE(i.has_number()); REQUIRE(i.total().value()==100); REQUIRE_FALSE(i.is_paid()); i.mark_paid(); REQUIRE(i.is_paid()); REQUIRE_FALSE(Invoice("",Money()).has_number()); }
