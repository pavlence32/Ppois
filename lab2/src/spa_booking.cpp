#include "spa_booking.hpp"
SpaBooking::SpaBooking(std::string procedure, Money price)
    : procedure_(procedure), price_(price), completed_(false) {}
void SpaBooking::complete() { completed_ = true; }
bool SpaBooking::is_completed() const { return completed_; }
Money SpaBooking::price() const { return price_; }
