#include "factory/AuditLog.hpp"

#include "core/exceptions.hpp"

#include <algorithm>

AuditLog::AuditLog(std::string actor, int day)
    : actor_(std::move(actor)), action_("created"), day_(day)
{
    entries_.push_back(action_);
}

void AuditLog::record(const std::string& action)
{
    if (action.empty())
        throw FactoryException("Действие для журнала не задано");
    action_ = action;
    entries_.push_back(action_);
}

int AuditLog::count_actions(const std::string& action) const
{
    return static_cast<int>(std::count(entries_.begin(), entries_.end(), action));
}

std::string AuditLog::last_entry() const
{
    if (entries_.empty())
        throw EntityNotFoundException("Журнал пуст");
    return entries_.back();
}

bool AuditLog::has_actor(const std::string& actor) const
{
    return actor_ == actor;
}

int AuditLog::size() const
{
    return static_cast<int>(entries_.size());
}
