#pragma once

#include "core/Date.hpp"
#include "core/Money.hpp"

#include <string>

struct Address
{
    std::string city;
    std::string street;
    std::string building;
};

enum class RoomStatus
{
    Free,
    Reserved,
    Occupied,
    Dirty
};

enum class AmenityCategory
{
    Comfort,
    Food,
    Technology,
    Service,
    Other
};

enum class ViewType
{
    City,
    Sea,
    Park
};

enum class ReservationStatus {
    Pending, Confirmed, CheckedIn, Cancelled, NoShow, Completed,
    PENDING = Pending, CONFIRMED = Confirmed, CHECKED_IN = CheckedIn,
    CANCELLED = Cancelled, NO_SHOW = NoShow, COMPLETED = Completed
};
enum class StayStatus {
    Active, Completed, ACTIVE = Active, COMPLETED = Completed
};
enum class RequestStatus {
    Open, Fulfilled, OPEN = Open, FULFILLED = Fulfilled
};
