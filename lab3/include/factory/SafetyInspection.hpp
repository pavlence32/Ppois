#pragma once

#include <string>

class SafetyInspection
{
    int id_;
    std::string area_;
    int score_;
    std::string inspector_;
    bool passed_;

public:
    SafetyInspection(int id, std::string area, int score, std::string inspector);

    bool is_critical() const;
    void recheck(int new_score);
    std::string grade() const;
    bool requires_follow_up() const;
    bool passed() const;
};
