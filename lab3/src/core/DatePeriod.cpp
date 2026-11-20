#include "core/DatePeriod.hpp"

#include "core/exceptions.hpp"

DatePeriod::DatePeriod(int start_day, int end_day)
    : start_day_(start_day), end_day_(end_day)
{
}

int DatePeriod::duration_days() const
{
    if (!is_valid())
        throw InvalidDateRangeException("Период заканчивается раньше, чем начинается");
    return end_day_ - start_day_ + 1;
}

bool DatePeriod::overlaps(const DatePeriod& other) const
{
    return start_day_ <= other.end_day_ && other.start_day_ <= end_day_;
}

bool DatePeriod::contains(int day) const
{
    return day >= start_day_ && day <= end_day_;
}

DatePeriod DatePeriod::shifted_by(int days) const
{
    return DatePeriod(start_day_ + days, end_day_ + days);
}

bool DatePeriod::is_valid() const
{
    return start_day_ <= end_day_;
}

int DatePeriod::start_day() const
{
    return start_day_;
}

int DatePeriod::end_day() const
{
    return end_day_;
}
