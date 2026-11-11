#include <catch2/catch_test_macros.hpp>
#include "finance/payment.hpp"
TEST_CASE("Payment") { Payment p(Money(100),"card"), cash(Money(10),"cash"); REQUIRE(p.amount().value()==100); REQUIRE(p.is_card_payment()); REQUIRE_FALSE(cash.is_card_payment()); REQUIRE_FALSE(p.is_completed()); p.complete(); REQUIRE(p.is_completed()); }
