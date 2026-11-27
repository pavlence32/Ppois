#pragma once

#include <string>
#include <vector>

class AuditLog
{
    std::string actor_;
    std::string action_;
    int day_;
    std::vector<std::string> entries_;

public:
    AuditLog(std::string actor, int day);

    void record(const std::string& action);
    int count_actions(const std::string& action) const;
    std::string last_entry() const;
    bool has_actor(const std::string& actor) const;
    int size() const;
};
