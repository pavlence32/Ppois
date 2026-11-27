#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "core/Address.hpp"
#include "core/ContactInfo.hpp"
#include "core/DatePeriod.hpp"
#include "core/Money.hpp"
#include "core/enums.hpp"
#include "core/exceptions.hpp"
#include "sales/Contract.hpp"
#include "sales/Customer.hpp"
#include "sales/Invoice.hpp"
#include "sales/SalesOrder.hpp"
#include "sales/Shipment.hpp"

namespace
{
Address customer_address()
{
    return Address("Market street", "Minsk", "220000", "Belarus", "3");
}

ContactInfo customer_contact()
{
    return ContactInfo("+375291234567", "client@mail.by", "office 7");
}
} // namespace

TEST_CASE("OrderItem calculates totals and discounts")
{
    const OrderItem item(1, 2, Money(100.0));

    REQUIRE(item.is_valid());
    REQUIRE(item.total().value() == Catch::Approx(200.0));
    REQUIRE(item.discounted_total(10.0).value() == Catch::Approx(180.0));
    REQUIRE(item.discounted_total(0.0).value() == Catch::Approx(200.0));
    REQUIRE_THROWS_AS(item.discounted_total(-1.0), InvalidMoneyException);
    REQUIRE_THROWS_AS(item.discounted_total(101.0), InvalidMoneyException);

    REQUIRE_FALSE(OrderItem(0, 1, Money(10.0)).is_valid());
    REQUIRE_FALSE(OrderItem(1, 0, Money(10.0)).is_valid());
    REQUIRE_FALSE(OrderItem(1, 1, Money(0.0)).is_valid());
}

TEST_CASE("SalesOrder registers items and payments")
{
    SalesOrder order(1, 10, 5);

    REQUIRE(order.customer_id() == 10);
    REQUIRE(order.status() == OrderStatus::Draft);
    REQUIRE(order.item_count() == 0);
    REQUIRE(order.total().value() == 0.0);
    REQUIRE_THROWS_AS(order.confirm(), InvalidOrderStateException);
    REQUIRE_THROWS_AS(order.add_item(OrderItem(0, 1, Money(10.0))), FactoryException);

    order.add_item(OrderItem(1, 2, Money(100.0)));
    order.add_item(OrderItem(2, 1, Money(50.0)));
    REQUIRE(order.item_count() == 2);
    REQUIRE(order.total().value() == Catch::Approx(250.0));

    order.confirm();
    REQUIRE(order.status() == OrderStatus::Confirmed);
    REQUIRE_THROWS_AS(order.confirm(), InvalidOrderStateException);
    REQUIRE_THROWS_AS(order.add_item(OrderItem(3, 1, Money(10.0))), InvalidOrderStateException);
    REQUIRE_THROWS_AS(order.register_payment(Money(0.0)), InvalidMoneyException);
    REQUIRE_THROWS_AS(order.register_payment(Money(251.0)), InvalidMoneyException);

    order.register_payment(Money(100.0));
    REQUIRE(order.outstanding().value() == Catch::Approx(150.0));
    REQUIRE_FALSE(order.is_paid());

    order.register_payment(Money(150.0));
    REQUIRE(order.is_paid());
    REQUIRE(order.outstanding().value() == Catch::Approx(0.0));
    REQUIRE(order.status() == OrderStatus::Completed);
    REQUIRE_THROWS_AS(order.cancel(), InvalidOrderStateException);

    REQUIRE_THROWS_AS(SalesOrder(2, 0, 0), EntityNotFoundException);

    SalesOrder cancelled(3, 10, 0);
    cancelled.cancel();
    REQUIRE(cancelled.status() == OrderStatus::Cancelled);
}

TEST_CASE("Customer respects credit limits and payments")
{
    Customer customer(1, "ACME", customer_address(), customer_contact(), Money(1000.0));

    REQUIRE(customer.name() == "ACME");
    REQUIRE_FALSE(customer.is_over_limit());
    REQUIRE(customer.available_credit().value() == Catch::Approx(1000.0));
    REQUIRE(customer.can_order(Money(500.0)));
    REQUIRE_FALSE(customer.can_order(Money(0.0)));
    REQUIRE_FALSE(customer.can_order(Money(1001.0)));

    customer.charge(Money(500.0));
    REQUIRE(customer.available_credit().value() == Catch::Approx(500.0));
    REQUIRE_THROWS_AS(customer.charge(Money(501.0)), CapacityExceededException);

    customer.pay(Money(500.0));
    REQUIRE(customer.available_credit().value() == Catch::Approx(1000.0));
    REQUIRE_THROWS_AS(customer.pay(Money(0.0)), InvalidMoneyException);

    customer.increase_credit(500.0);
    REQUIRE(customer.available_credit().value() == Catch::Approx(1500.0));
    REQUIRE_THROWS_AS(customer.increase_credit(0.0), InvalidMoneyException);

    REQUIRE_THROWS_AS(Customer(2, "Bad", customer_address(), customer_contact(), Money(0.0)),
                      InvalidMoneyException);
}

TEST_CASE("Contract validates terms, signing and penalties")
{
    Contract contract(1, 10, DatePeriod(0, 30), Money(1000.0));

    REQUIRE_FALSE(contract.is_signed());
    REQUIRE_FALSE(contract.is_active(10));
    REQUIRE(contract.penalty(0).value() == 0.0);
    REQUIRE(contract.penalty(-5).value() == 0.0);

    contract.sign();
    REQUIRE(contract.is_signed());
    REQUIRE(contract.is_active(0));
    REQUIRE(contract.is_active(30));
    REQUIRE_FALSE(contract.is_active(31));
    REQUIRE_THROWS_AS(contract.sign(), InvalidOrderStateException);

    REQUIRE(contract.penalty(4).value() == Catch::Approx(20.0));
    REQUIRE(contract.penalty(100).value() == Catch::Approx(200.0));

    SalesOrder matching(1, 10, 0);
    matching.add_item(OrderItem(1, 1, Money(900.0)));
    REQUIRE(contract.matches_order(matching));

    SalesOrder expensive(2, 10, 0);
    expensive.add_item(OrderItem(1, 1, Money(1100.0)));
    REQUIRE_FALSE(contract.matches_order(expensive));

    SalesOrder other_customer(3, 11, 0);
    REQUIRE_FALSE(contract.matches_order(other_customer));

    Contract unsigned_contract(2, 10, DatePeriod(0, 5), Money(0.0));
    REQUIRE_THROWS_AS(unsigned_contract.sign(), InvalidMoneyException);
    REQUIRE_THROWS_AS(Contract(3, 10, DatePeriod(10, 0), Money(100.0)),
                      InvalidDateRangeException);
}

TEST_CASE("Shipment tracks dispatch, delivery and loss")
{
    Shipment shipment(1, 10, "Gomel");

    REQUIRE(shipment.status() == ShipmentStatus::Prepared);
    REQUIRE_FALSE(shipment.is_delivered());
    REQUIRE(shipment.transit_days() == 0);
    REQUIRE_THROWS_AS(shipment.deliver(5), InvalidOrderStateException);
    REQUIRE_THROWS_AS(shipment.dispatch(-1), InvalidDateRangeException);

    shipment.dispatch(2);
    REQUIRE(shipment.status() == ShipmentStatus::InTransit);
    REQUIRE_THROWS_AS(shipment.dispatch(3), InvalidOrderStateException);
    REQUIRE_THROWS_AS(shipment.deliver(1), InvalidDateRangeException);

    shipment.deliver(5);
    REQUIRE(shipment.status() == ShipmentStatus::Delivered);
    REQUIRE(shipment.is_delivered());
    REQUIRE(shipment.transit_days() == 3);
    REQUIRE(shipment.is_late(2));
    REQUIRE_FALSE(shipment.is_late(3));
    REQUIRE_THROWS_AS(shipment.mark_lost(), InvalidOrderStateException);

    Shipment lost(2, 11, "Brest");
    lost.mark_lost();
    REQUIRE(lost.status() == ShipmentStatus::Lost);
}

TEST_CASE("Invoice accepts payments only after issuing")
{
    Invoice invoice(1, 10, Money(1000.0), 10);

    REQUIRE_FALSE(invoice.is_issued());
    REQUIRE(invoice.balance().value() == Catch::Approx(1000.0));
    REQUIRE_FALSE(invoice.is_overdue(20));
    REQUIRE_THROWS_AS(invoice.apply_payment(Money(100.0)), InvalidOrderStateException);

    invoice.issue();
    REQUIRE(invoice.is_issued());
    REQUIRE_THROWS_AS(invoice.issue(), InvalidOrderStateException);
    REQUIRE(invoice.is_overdue(11));
    REQUIRE_FALSE(invoice.is_overdue(10));
    REQUIRE_THROWS_AS(invoice.apply_payment(Money(0.0)), InvalidMoneyException);
    REQUIRE_THROWS_AS(invoice.apply_payment(Money(1001.0)), InvalidMoneyException);

    invoice.apply_payment(Money(400.0));
    REQUIRE(invoice.balance().value() == Catch::Approx(600.0));
    REQUIRE_FALSE(invoice.is_settled());

    invoice.apply_payment(Money(600.0));
    REQUIRE(invoice.is_settled());
    REQUIRE_FALSE(invoice.is_overdue(20));

    REQUIRE_THROWS_AS(Invoice(2, 10, Money(0.0), 10), InvalidMoneyException);
}

TEST_CASE("Payment confirms, refunds and formats receipts")
{
    Payment payment(1, 10, Money(500.0), "card");

    REQUIRE(payment.amount().value() == Catch::Approx(500.0));
    REQUIRE_FALSE(payment.is_confirmed());
    REQUIRE_THROWS_AS(payment.refund(), InvalidOrderStateException);
    REQUIRE(payment.is_large(500.0));
    REQUIRE_FALSE(payment.is_large(501.0));
    REQUIRE(payment.receipt().find("ожидает") != std::string::npos);

    payment.confirm();
    REQUIRE(payment.is_confirmed());
    REQUIRE_THROWS_AS(payment.confirm(), InvalidOrderStateException);
    REQUIRE(payment.receipt().find("подтверждён") != std::string::npos);
    REQUIRE(payment.receipt().find("card") != std::string::npos);

    payment.refund();
    REQUIRE_FALSE(payment.is_confirmed());

    REQUIRE_THROWS_AS(Payment(2, 10, Money(0.0), "cash"), InvalidMoneyException);
}
