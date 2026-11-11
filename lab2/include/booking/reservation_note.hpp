#pragma once

#include "core/model.hpp"
#include <string>

class ReservationNote
{
    std::string text_;
    Date created_at_;
    bool important_;

public:
    ReservationNote(std::string text, bool important);
    bool is_important() const;
    bool is_empty() const;
    bool was_created_before(Date date) const;
};
