#include "conference_booking.hpp"
ConferenceBooking::ConferenceBooking(int seats, Money price)
    : seats_(seats), price_(price), reserved_(false) {}
void ConferenceBooking::reserve() { reserved_ = true; }
bool ConferenceBooking::is_reserved() const { return reserved_; }
Money ConferenceBooking::total() const { return price_ * seats_; }
