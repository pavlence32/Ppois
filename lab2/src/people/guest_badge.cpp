#include "people/guest_badge.hpp"

GuestBadge::GuestBadge(std::string guest_name)
    : guest_name_(guest_name), active_(true)
{}

void GuestBadge::deactivate()
{
    active_ = false;
}

bool GuestBadge::is_active() const
{
    return active_;
}

bool GuestBadge::belongs_to(const std::string& guest_name) const
{
    return guest_name_ == guest_name;
}
