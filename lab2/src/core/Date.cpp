#include "core/Date.hpp"

bool valid_date_range(Date start, Date end)
{
    return start < end;
}

Date now_date()
{
    return std::chrono::system_clock::now();
}

int days_between(Date start, Date end)
{
    return static_cast<int>(
        std::chrono::duration_cast<std::chrono::hours>(end - start).count() / 24);
}

bool date_has_passed(Date date, Date now)
{
    return date < now;
}
