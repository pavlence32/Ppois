#pragma once

#include "core/enums.hpp"

#include <string>

class Notification
{
    int id_;
    std::string recipient_;
    std::string message_;
    Priority priority_;
    bool read_;

public:
    Notification(int id, std::string recipient, std::string message, Priority priority);

    void mark_read();
    std::string format() const;
    bool is_urgent() const;
    void escalate();
    bool is_read() const;
};
