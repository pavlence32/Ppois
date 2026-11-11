#pragma once
#include "core/model.hpp"
class KeyCard
{
    int room_number_;
    Date expires_at_;
    bool active_;
public:
    KeyCard(int room_number, Date expires_at);
    void deactivate();
    bool is_active() const;
    bool is_valid(Date now) const;
};
