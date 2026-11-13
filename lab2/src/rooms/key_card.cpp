#include "rooms/key_card.hpp"
KeyCard::KeyCard(int room_number, Date expires_at)
    : room_number_(room_number), expires_at_(expires_at), active_(true) {}
void KeyCard::deactivate() { active_ = false; }
bool KeyCard::is_active() const { return active_; }
bool KeyCard::is_valid(Date now) const
{
    return active_ && now < expires_at_;
}
