#include "booking/reservation_note.hpp"

ReservationNote::ReservationNote(std::string text, bool important)
    : text_(text), created_at_(now_date()), important_(important)
{}

bool ReservationNote::is_important() const { return important_; }
bool ReservationNote::is_empty() const { return text_.empty(); }
bool ReservationNote::was_created_before(Date date) const
{
    return created_at_ < date;
}
