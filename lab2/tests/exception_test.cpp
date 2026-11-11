#include <catch2/catch_test_macros.hpp>
#include "core/exception.hpp"
TEST_CASE("Exception preserves message") { Exception e("message"); REQUIRE(e.getMessage()=="message"); }
TEST_CASE("All domain exceptions preserve messages") {
#define CHECK_EXCEPTION(type) { type e("x"); REQUIRE(e.getMessage()=="x"); }
CHECK_EXCEPTION(EmployeeUnavailableException); CHECK_EXCEPTION(InvalidDatesException); CHECK_EXCEPTION(RoomNotAvailableException); CHECK_EXCEPTION(RoomNotCleanedException); CHECK_EXCEPTION(RoomNotFoundException); CHECK_EXCEPTION(RoomOverCapacityException); CHECK_EXCEPTION(DuplicateRoomException); CHECK_EXCEPTION(InvalidKeyException); CHECK_EXCEPTION(KeyExpiredException); CHECK_EXCEPTION(InvalidDateException); CHECK_EXCEPTION(InvalidPaymentException); CHECK_EXCEPTION(InsufficientFundsException); CHECK_EXCEPTION(OverbookingException); CHECK_EXCEPTION(InvalidReservationStateException); CHECK_EXCEPTION(CancellationNotAllowedException); CHECK_EXCEPTION(ReservationNotFoundException); CHECK_EXCEPTION(WaitListFullException);
#undef CHECK_EXCEPTION
}
