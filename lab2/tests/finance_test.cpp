#include <catch2/catch_test_macros.hpp>
#include "finance/discount.hpp"
TEST_CASE("Discount") { auto n=now_date(); Discount d("sale",20,n+std::chrono::hours(24)); REQUIRE(d.apply(Money(100)).value()==80); REQUIRE(d.is_valid(n)); REQUIRE_FALSE(d.is_valid(n+std::chrono::hours(48))); REQUIRE_FALSE(d.is_full()); Discount full("full",100,n); REQUIRE(full.is_full()); }
#include <catch2/catch_test_macros.hpp>
#include "core/exception.hpp"
TEST_CASE("InsufficientFundsException stores message") { InsufficientFundsException e("x"); REQUIRE(e.getMessage() == "x"); }
#include <catch2/catch_test_macros.hpp>
#include "finance/invoice.hpp"
TEST_CASE("Invoice") { Invoice i("INV-1",Money(100)); REQUIRE(i.has_number()); REQUIRE(i.total().value()==100); REQUIRE_FALSE(i.is_paid()); i.mark_paid(); REQUIRE(i.is_paid()); REQUIRE_FALSE(Invoice("",Money()).has_number()); }
#include <catch2/catch_test_macros.hpp>
#include "core/Date.hpp"
#include "core/Money.hpp"
TEST_CASE("Money arithmetic and predicates") { Money a(100), b(25); REQUIRE(a.value()==100); REQUIRE((a * 2.5).value()==250); REQUIRE((a+b).value()==125); REQUIRE((a-b).value()==75); REQUIRE(a.percent(10).value()==10); REQUIRE(a.add_tax(20).value()==120); REQUIRE(a.apply_discount(15).value()==85); REQUIRE_FALSE(a.is_zero()); REQUIRE(a.is_positive()); REQUIRE(Money().is_zero()); REQUIRE_FALSE(Money(-1).is_positive()); }
TEST_CASE("Date helpers") { auto n=now_date(); auto later=n+std::chrono::hours(48); REQUIRE(valid_date_range(n,later)); REQUIRE_FALSE(valid_date_range(later,n)); REQUIRE(days_between(n,later)==2); REQUIRE(date_has_passed(n-std::chrono::hours(1),n)); REQUIRE_FALSE(date_has_passed(later,n)); }
#include <catch2/catch_test_macros.hpp>
#include "finance/payment.hpp"
TEST_CASE("Payment") { Payment p(Money(100),"card"), cash(Money(10),"cash"); REQUIRE(p.amount().value()==100); REQUIRE(p.is_card_payment()); REQUIRE_FALSE(cash.is_card_payment()); REQUIRE_FALSE(p.is_completed()); p.complete(); REQUIRE(p.is_completed()); }
