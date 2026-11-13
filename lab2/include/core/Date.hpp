#pragma once

#include <chrono>

using Date = std::chrono::system_clock::time_point;

bool valid_date_range(Date start, Date end);
Date now_date();
int days_between(Date start, Date end);
bool date_has_passed(Date date, Date now);
