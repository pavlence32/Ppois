#include "booking/Reservation.hpp"

#include "booking/AvailabilityCalendar.hpp"
#include "booking/BookingPolicy.hpp"
#include "core/exception.hpp"
#include "rooms/room.hpp"

#include <utility>

Reservation::Reservation(std::string id, Guest* guest, Room* room, Date check_in,
                         Date check_out, int guests_count, BookingPolicy* policy)
    : id_(std::move(id)), guest_(guest), room_(room), check_in_(check_in),
      check_out_(check_out), guests_count_(guests_count),
      status_(ReservationStatus::Pending), total_price_(0.0),
      created_at_(now_date()), policy_(policy), calendar_(nullptr)
{
    if (!valid_date_range(check_in_, check_out_) || guests_count_ <= 0)
        throw InvalidDatesException{"Некорректные даты или число гостей"};
}

Reservation::Reservation(int id, Guest* guest, Room* room, Date check_in,
                         Date check_out, int guests_count, BookingPolicy* policy)
    : Reservation(std::to_string(id), guest, room, check_in, check_out,
                  guests_count, policy)
{}

const std::string& Reservation::id() const { return id_; }
Guest* Reservation::guest() const { return guest_; }
Room* Reservation::room() const { return room_; }
Date Reservation::check_in() const { return check_in_; }
Date Reservation::check_out() const { return check_out_; }
int Reservation::guests_count() const { return guests_count_; }
ReservationStatus Reservation::status() const { return status_; }
Money Reservation::total_price() const { return total_price_; }
Date Reservation::created_at() const { return created_at_; }
BookingPolicy* Reservation::policy() const { return policy_; }

void Reservation::confirm(AvailabilityCalendar& calendar)
{
    if (status_ != ReservationStatus::Pending)
        throw InvalidReservationStateException{"Бронь уже обработана"};
    room_->can_accommodate(guests_count_);
    if (!calendar.is_free(room_, check_in_, check_out_))
        throw OverbookingException{"Номер занят на выбранные даты"};
    calendar.book(this);
    calendar_ = &calendar;
    try {
        room_->reserve();
    } catch (const RoomNotAvailableException&) {
        calendar.release(this);
        throw OverbookingException{"Номер недоступен"};
    }
    total_price_ = calculate_total();
    status_ = ReservationStatus::Confirmed;
}

Money Reservation::cancel(Date now)
{
    if (policy_ == nullptr || !policy_->can_cancel(*this, now))
        throw CancellationNotAllowedException{"Отмена брони невозможна"};
    Money penalty = policy_->calculate_penalty(*this, now);
    if (status_ == ReservationStatus::Confirmed) {
        if (calendar_ != nullptr)
            calendar_->release(this);
        room_->cancel_reservation();
    }
    status_ = ReservationStatus::Cancelled;
    return penalty;
}

void Reservation::extend(Date new_check_out, AvailabilityCalendar& calendar)
{
    if (status_ != ReservationStatus::Confirmed &&
        status_ != ReservationStatus::CheckedIn)
        throw InvalidReservationStateException{"Продлить можно только активную бронь"};
    if (!valid_date_range(check_in_, new_check_out) || new_check_out <= check_out_)
        throw InvalidDatesException{"Некорректная дата продления"};
    if (!calendar.is_free(room_, check_out_, new_check_out))
        throw OverbookingException{"Номер занят на дополнительные даты"};
    check_out_ = new_check_out;
    total_price_ = calculate_total();
}

int Reservation::nights() const
{
    if (!valid_date_range(check_in_, check_out_))
        throw InvalidDatesException{"Некорректные даты брони"};
    return static_cast<int>(std::chrono::duration_cast<std::chrono::hours>(
        check_out_ - check_in_).count() / 24);
}

Money Reservation::calculate_total() const
{
    return room_->calculate_price(nights());
}

bool Reservation::overlaps(Date start, Date end) const
{
    return is_active() && start < check_out_ && check_in_ < end;
}

bool Reservation::is_active() const
{
    return status_ == ReservationStatus::Confirmed ||
           status_ == ReservationStatus::CheckedIn;
}

void Reservation::mark_no_show(Date now)
{
    if (status_ != ReservationStatus::Confirmed || now <= check_in_)
        throw InvalidReservationStateException{"Бронь нельзя отметить как неявившуюся"};
    status_ = ReservationStatus::NoShow;
}

bool Reservation::can_check_in(Date now) const
{
    return status_ == ReservationStatus::Confirmed && now >= check_in_ &&
           now < check_out_;
}

bool Reservation::can_cancel(Date now) const
{
    return is_active() && (policy_ == nullptr || policy_->can_cancel(*this, now));
}

void Reservation::change_guest_count(int guests)
{
    if (guests <= 0)
        throw RoomOverCapacityException{"Число гостей должно быть положительным"};
    room_->can_accommodate(guests);
    guests_count_ = guests;
    total_price_ = calculate_total();
}

Money Reservation::price_per_night() const
{
    return room_->calculate_price(1);
}

bool Reservation::is_finished() const
{
    return status_ == ReservationStatus::Cancelled ||
           status_ == ReservationStatus::NoShow ||
           status_ == ReservationStatus::Completed;
}
