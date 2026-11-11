#include "finance/payment.hpp"

Payment::Payment(Money amount, std::string method)
    : amount_(amount), method_(method), completed_(false)
{}

void Payment::complete() { completed_ = true; }
bool Payment::is_completed() const { return completed_; }
bool Payment::is_card_payment() const { return method_ == "card"; }
Money Payment::amount() const { return amount_; }
