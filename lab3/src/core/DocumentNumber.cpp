#include "core/DocumentNumber.hpp"

#include <iomanip>
#include <sstream>

DocumentNumber::DocumentNumber(std::string prefix, int sequence)
    : prefix_(std::move(prefix)), sequence_(sequence)
{
}

std::string DocumentNumber::next()
{
    std::ostringstream stream;
    stream << prefix_ << '-' << std::setw(4) << std::setfill('0') << sequence_++;
    return stream.str();
}

std::string DocumentNumber::formatted() const
{
    std::ostringstream stream;
    stream << prefix_ << '-' << std::setw(4) << std::setfill('0') << sequence_;
    return stream.str();
}

bool DocumentNumber::matches(const std::string& candidate) const
{
    const std::string prefix = prefix_ + "-";
    return candidate.rfind(prefix, 0) == 0 && candidate.size() >= prefix.size() + 4;
}

void DocumentNumber::reset(int sequence)
{
    sequence_ = sequence;
}
