#pragma once

#include <string>

class Complaint
{
    std::string text_;
    bool resolved_;
    int priority_;

public:
    Complaint(std::string text, int priority);
    void resolve();
    bool is_open() const;
    bool is_urgent() const;
};
