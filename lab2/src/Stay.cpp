#include "Stay.hpp"

#include "AvailabilityCalendar.hpp"
#include "Reservation.hpp"
#include "room_key.hpp"
#include "exception.hpp"
#include "hotel.hpp"
#include "room.hpp"

#include <algorithm>
#include <sstream>

Stay::Stay(Reservation* reservation)
    : reservation_(reservation), key_(nullptr), actual_check_in_{},
      actual_check_out_{}, status_(StayStatus::Active)
{
    if (reservation_ == nullptr)
        throw InvalidReservationStateException{"Проживание требует бронь"};
}

void Stay::check_in(Date now)
{
    if (status_ != StayStatus::Active ||
        reservation_->status() != ReservationStatus::Confirmed ||
        now < reservation_->check_in())
        throw InvalidReservationStateException{"Заселение невозможно"};
    reservation_->room()->occupy();
    key_ = new RoomKey("key-" + reservation_->id(), reservation_->room(),
                       reservation_->guest(), now, reservation_->check_out());
    actual_check_in_ = now;
    reservation_->status_ = ReservationStatus::CheckedIn;
}

void Stay::check_out(Date now)
{
    if (status_ != StayStatus::Active ||
        reservation_->status() != ReservationStatus::CheckedIn)
        throw InvalidReservationStateException{"Выселение невозможно"};
    reservation_->room()->release();
    key_->deactivate();
    actual_check_out_ = now;
    reservation_->status_ = ReservationStatus::Completed;
    status_ = StayStatus::Completed;
}

static Date checkout_deadline(Date date, const std::string& value)
{
    int hours = 12;
    int minutes = 0;
    std::istringstream input(value);
    char separator = 0;
    input >> hours >> separator >> minutes;
    const auto day = std::chrono::time_point_cast<std::chrono::hours>(date);
    return day + std::chrono::hours(hours) + std::chrono::minutes(minutes);
}

bool Stay::is_late_checkout(Date now, const Hotel& hotel) const
{
    return status_ == StayStatus::Active &&
           now > checkout_deadline(reservation_->check_out(),
                                   hotel.get_check_out_time());
}

Money Stay::calculate_late_fee(Date now, const Hotel& hotel) const
{
    if (!is_late_checkout(now, hotel))
        return Money{0.0};
    const auto deadline = checkout_deadline(reservation_->check_out(),
                                            hotel.get_check_out_time());
    const auto hours = std::chrono::duration_cast<std::chrono::hours>(
        now - deadline).count();
    return reservation_->room()->calculate_price(1) *
           (hours > 3 ? 0.5 : 0.0);
}

int Stay::actual_nights() const
{
    const Date end = status_ == StayStatus::Completed ? actual_check_out_
                                                       : now_date();
    if (actual_check_in_ == Date{} || end <= actual_check_in_)
        return 0;
    return std::max(1, static_cast<int>(
        std::chrono::duration_cast<std::chrono::hours>(
            end - actual_check_in_).count() / 24));
}

void Stay::extend_stay(Date new_date, AvailabilityCalendar& calendar)
{
    if (status_ != StayStatus::Active ||
        reservation_->status() != ReservationStatus::CheckedIn)
        throw InvalidReservationStateException{"Продление проживания невозможно"};
    reservation_->extend(new_date, calendar);
    key_->extend(new_date);
}

Reservation* Stay::reservation() const { return reservation_; }
RoomKey* Stay::key() const { return key_; }
StayStatus Stay::status() const { return status_; }
