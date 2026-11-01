#include "GuestRequest.hpp"

#include "exception.hpp"
#include "room.hpp"

GuestRequest::GuestRequest(Guest* guest, Date check_in, Date check_out,
                           int guests_count,
                           std::vector<std::string> required_amenities,
                           Money max_price_per_night)
    : guest_(guest), check_in_(check_in), check_out_(check_out),
      guests_count_(guests_count),
      required_amenities_(std::move(required_amenities)),
      max_price_per_night_(max_price_per_night), created_at_(now_date()),
      status_(RequestStatus::Open)
{
    if (!valid_date_range(check_in_, check_out_) || guests_count_ <= 0)
        throw InvalidDatesException{"Некорректные даты или число гостей"};
}

bool GuestRequest::matches(Room* room) const
{
    if (room == nullptr)
        return false;
    try {
        room->can_accommodate(guests_count_);
    } catch (const RoomOverCapacityException&) {
        return false;
    }
    for (const std::string& amenity : required_amenities_)
        if (!room->has_amenity(amenity))
            return false;
    return room->calculate_price(1).value() <= max_price_per_night_.value();
}

bool GuestRequest::is_expired(Date now) const { return now >= check_in_; }

int GuestRequest::nights() const
{
    return static_cast<int>(std::chrono::duration_cast<std::chrono::hours>(
        check_out_ - check_in_).count() / 24);
}

void GuestRequest::fulfill()
{
    if (status_ != RequestStatus::Open)
        throw InvalidReservationStateException{"Заявка уже выполнена"};
    status_ = RequestStatus::Fulfilled;
}

Guest* GuestRequest::guest() const { return guest_; }
Date GuestRequest::check_in() const { return check_in_; }
Date GuestRequest::check_out() const { return check_out_; }
int GuestRequest::guests_count() const { return guests_count_; }
RequestStatus GuestRequest::status() const { return status_; }
