#include "factory/Notification.hpp"

#include "core/exceptions.hpp"

Notification::Notification(int id, std::string recipient, std::string message, Priority priority)
    : id_(id),
      recipient_(std::move(recipient)),
      message_(std::move(message)),
      priority_(priority),
      read_(false)
{
    if (recipient_.empty() || message_.empty())
        throw FactoryException("Уведомление должно иметь получателя и текст");
}

void Notification::mark_read()
{
    read_ = true;
}

std::string Notification::format() const
{
    return "[" + std::to_string(static_cast<int>(priority_)) + "] " + recipient_ + ": "
        + message_;
}

bool Notification::is_urgent() const
{
    return priority_ == Priority::High || priority_ == Priority::Critical;
}

void Notification::escalate()
{
    if (priority_ != Priority::Critical)
        priority_ = static_cast<Priority>(static_cast<int>(priority_) + 1);
}

bool Notification::is_read() const
{
    return read_;
}
