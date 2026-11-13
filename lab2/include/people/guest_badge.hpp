#pragma once
#include <string>
class GuestBadge
{
    std::string guest_name_;
    bool active_;
public:
    explicit GuestBadge(std::string guest_name);
    void deactivate();
    bool is_active() const;
    bool belongs_to(const std::string& guest_name) const;
};
