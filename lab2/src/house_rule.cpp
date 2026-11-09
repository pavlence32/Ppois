#include "house_rule.hpp"

HouseRule::HouseRule(std::string title, std::string description)
    : title_(title), description_(description), active_(true)
{}

void HouseRule::disable() { active_ = false; }
bool HouseRule::is_active() const { return active_; }
bool HouseRule::applies_to(const std::string& title) const
{
    return title_ == title;
}
