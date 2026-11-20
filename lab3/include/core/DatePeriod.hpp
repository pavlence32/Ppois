#pragma once

class DatePeriod
{
    int start_day_;
    int end_day_;

public:
    DatePeriod(int start_day = 0, int end_day = 0);

    int duration_days() const;
    bool overlaps(const DatePeriod& other) const;
    bool contains(int day) const;
    DatePeriod shifted_by(int days) const;
    bool is_valid() const;
    int start_day() const;
    int end_day() const;
};
