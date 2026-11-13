#pragma once
#include <string>
class GuestFeedback
{
    std::string text_;
    int rating_;
    bool processed_;
public:
    GuestFeedback(std::string text, int rating);
    void process();
    bool is_processed() const;
    bool is_positive() const;
};
