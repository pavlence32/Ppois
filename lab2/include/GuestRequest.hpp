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
    bool urgent_ = false;
    bool contacted_ = false;
    int priority_ = 0;
    std::string note_;
    std::string contact_method_;
    int contact_attempts_ = 0;
    bool flexible_dates_ = false;
    bool flexible_price_ = false;
    std::string preferred_room_type_;
    std::string preferred_floor_;
    std::string accessibility_note_;
    std::string response_note_;
    std::string assigned_manager_;

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
    bool needs_amenity(const std::string& name) const;
    bool accepts_price(Money price) const;
    void cancel();
};
