#include "people/review.hpp"

Review::Review(int rating, std::string text)
    : rating_(rating), text_(text), published_(false)
{}

void Review::publish() { published_ = true; }
bool Review::is_positive() const { return rating_ >= 4; }
bool Review::is_published() const { return published_; }
