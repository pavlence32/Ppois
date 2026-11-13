#include "finance/invoice.hpp"

Invoice::Invoice(std::string number, Money total)
    : number_(number), total_(total), paid_(false)
{}

void Invoice::mark_paid() { paid_ = true; }
bool Invoice::is_paid() const { return paid_; }
bool Invoice::has_number() const { return !number_.empty(); }
Money Invoice::total() const { return total_; }
