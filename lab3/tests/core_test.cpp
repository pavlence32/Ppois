#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "core/Address.hpp"
#include "core/ContactInfo.hpp"
#include "core/DatePeriod.hpp"
#include "core/DocumentNumber.hpp"
#include "core/Money.hpp"
#include "core/exceptions.hpp"

TEST_CASE("Money performs arithmetic and percentages")
{
    const Money salary(1000.0);
    const Money bonus(250.0);

    REQUIRE((salary + bonus).value() == 1250.0);
    REQUIRE((salary - bonus).value() == 750.0);
    REQUIRE((salary * 2.0).value() == 2000.0);
    REQUIRE(salary.percent(10.0).value() == Catch::Approx(100.0));
    REQUIRE(salary.with_tax(20.0).value() == Catch::Approx(1200.0));
    REQUIRE(salary.with_discount(25.0).value() == Catch::Approx(750.0));

    REQUIRE(Money(0.0).is_zero());
    REQUIRE(salary.is_positive());
    REQUIRE_FALSE(Money(-1.0).is_positive());
    REQUIRE(salary.can_cover(bonus));
    REQUIRE_FALSE(bonus.can_cover(salary));

    Money total(100.0);
    total += bonus;
    REQUIRE(total.value() == 350.0);
}

TEST_CASE("Address formats and compares locations")
{
    const Address home("Main street", "Minsk", "220000", "Belarus", "12");

    REQUIRE(home.format() == "Belarus, Minsk, Main street 12 (220000)");
    REQUIRE(home.city() == "Minsk");
    REQUIRE(home.is_complete());

    const Address neighbour("Other street", "Minsk", "220001", "Belarus", "5");
    const Address abroad("Main street", "Warsaw", "00-001", "Poland", "12");
    REQUIRE(home.same_city(neighbour));
    REQUIRE_FALSE(home.same_city(abroad));

    const Address incomplete("", "Minsk", "", "Belarus", "");
    REQUIRE_FALSE(incomplete.is_complete());
}

TEST_CASE("ContactInfo validates email, phone and updates")
{
    const ContactInfo contact("+375 29 123 45 67", "engineer@factory.by", "room 42");

    REQUIRE(contact.has_valid_email());
    REQUIRE(contact.has_valid_phone());
    REQUIRE(contact.display() == "+375 29 123 45 67 / engineer@factory.by / room 42");
    REQUIRE(contact.email() == "engineer@factory.by");

    REQUIRE_FALSE(ContactInfo("", "not-an-email", "").has_valid_email());
    REQUIRE_FALSE(ContactInfo("123", "a@b.co", "").has_valid_phone());

    ContactInfo editable("+1234567", "a@b.co", "");
    editable.update_phone("+375291112233");
    REQUIRE(editable.display().find("+375291112233") == 0);
    REQUIRE_THROWS_AS(editable.update_phone("12"), FactoryException);
}

TEST_CASE("DatePeriod computes duration, overlap and shifts")
{
    const DatePeriod week(0, 6);

    REQUIRE(week.is_valid());
    REQUIRE(week.duration_days() == 7);
    REQUIRE(week.contains(0));
    REQUIRE(week.contains(6));
    REQUIRE_FALSE(week.contains(7));

    const DatePeriod weekend(5, 7);
    const DatePeriod next_week(7, 13);
    REQUIRE(week.overlaps(weekend));
    REQUIRE_FALSE(week.overlaps(next_week));
    REQUIRE_FALSE(weekend.overlaps(DatePeriod(8, 10)));

    const DatePeriod shifted = week.shifted_by(7);
    REQUIRE(shifted.start_day() == 7);
    REQUIRE(shifted.end_day() == 13);

    const DatePeriod invalid(10, 5);
    REQUIRE_FALSE(invalid.is_valid());
    REQUIRE_THROWS_AS(invalid.duration_days(), InvalidDateRangeException);
}

TEST_CASE("DocumentNumber issues sequential numbers")
{
    DocumentNumber number("INV", 7);

    REQUIRE(number.formatted() == "INV-0007");
    REQUIRE(number.next() == "INV-0007");
    REQUIRE(number.next() == "INV-0008");
    REQUIRE(number.formatted() == "INV-0009");
    REQUIRE(number.matches("INV-0009"));
    REQUIRE_FALSE(number.matches("INV-09"));
    REQUIRE_FALSE(number.matches("ORD-0009"));

    number.reset(1);
    REQUIRE(number.formatted() == "INV-0001");
}

TEST_CASE("Domain exceptions preserve their messages")
{
    const FactoryException base("base failure");
    REQUIRE(base.getMessage() == "base failure");

    const InvalidMoneyException money("bad money");
    const InvalidDateRangeException dates("bad dates");
    const EntityNotFoundException missing("missing");
    const DuplicateEntityException duplicate("duplicate");
    const UnauthorizedAccessException access("denied");
    const InsufficientStockException stock("no stock");
    const MachineUnavailableException machine("unavailable");
    const QualityCheckFailedException quality("failed");
    const InvalidOrderStateException state("bad state");
    const CapacityExceededException capacity("full");
    const InvalidShiftException shift("bad shift");
    const SafetyViolationException safety("unsafe");

    REQUIRE(money.getMessage() == "bad money");
    REQUIRE(dates.getMessage() == "bad dates");
    REQUIRE(missing.getMessage() == "missing");
    REQUIRE(duplicate.getMessage() == "duplicate");
    REQUIRE(access.getMessage() == "denied");
    REQUIRE(stock.getMessage() == "no stock");
    REQUIRE(machine.getMessage() == "unavailable");
    REQUIRE(quality.getMessage() == "failed");
    REQUIRE(state.getMessage() == "bad state");
    REQUIRE(capacity.getMessage() == "full");
    REQUIRE(shift.getMessage() == "bad shift");
    REQUIRE(safety.getMessage() == "unsafe");
}
