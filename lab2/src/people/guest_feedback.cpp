#include "people/guest_feedback.hpp"
GuestFeedback::GuestFeedback(std::string text, int rating)
    : text_(text), rating_(rating), processed_(false) {}
void GuestFeedback::process() { processed_ = true; }
bool GuestFeedback::is_processed() const { return processed_; }
bool GuestFeedback::is_positive() const { return rating_ >= 4; }
