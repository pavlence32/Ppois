#pragma once

#include "core/model.hpp"
#include <string>

class ReservationNote
{
    std::string text_;
    Date created_at_;
    bool important_;

public:
    ReservationNote() : text_(""), created_at_(now_date()), important_(false) {}
    ReservationNote(std::string text, bool important);
    bool is_important() const;
    bool is_empty() const;
    bool was_created_before(Date date) const;
};
