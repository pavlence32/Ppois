#pragma once

#include "model.hpp"
#include <string>
#include <vector>

class Guest;
class Room;

class GuestRequest
{
    Guest* guest_;
    Date check_in_;
    Date check_out_;
    int guests_count_;
    std::vector<std::string> required_amenities_;
    Money max_price_per_night_;
    Date created_at_;
    RequestStatus status_;

public:
    GuestRequest(Guest* guest, Date check_in, Date check_out, int guests_count,
                 std::vector<std::string> required_amenities, Money max_price_per_night);
    bool matches(Room* room) const;
    bool is_expired(Date now) const;
    int nights() const;
    void fulfill();
    Guest* guest() const;
    Date check_in() const;
    Date check_out() const;
    int guests_count() const;
    RequestStatus status() const;
};
