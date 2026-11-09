#pragma once

#include <string>

class HouseRule
{
    std::string title_;
    std::string description_;
    bool active_;

public:
    HouseRule(std::string title, std::string description);
    void disable();
    bool is_active() const;
    bool applies_to(const std::string& title) const;
};
