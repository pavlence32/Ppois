#pragma once

#include <chrono>
#include <string>

using Date = std::chrono::system_clock::time_point;

class Money
{
    double amount_;

public:
    explicit Money(double amount = 0.0);
    double value() const;
    Money operator*(double multiplier) const;
    Money operator+(const Money& other) const;
    Money operator-(const Money& other) const;
};

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

bool valid_date_range(Date start, Date end);
Date now_date();
