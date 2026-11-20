#pragma once

#include <string>

class DocumentNumber
{
    std::string prefix_;
    int sequence_;

public:
    DocumentNumber(std::string prefix = "DOC", int sequence = 1);

    std::string next();
    std::string formatted() const;
    bool matches(const std::string& candidate) const;
    void reset(int sequence);
};
