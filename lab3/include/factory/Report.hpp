#pragma once

#include "core/DatePeriod.hpp"
#include "core/Money.hpp"

#include <string>

class Report
{
    std::string title_;
    DatePeriod period_;
    Money revenue_;
    Money expenses_;
    std::string author_;

public:
    Report(std::string title,
           DatePeriod period,
           Money revenue,
           Money expenses,
           std::string author);

    Money profit() const;
    double margin() const;
    bool is_profitable() const;
    std::string summary() const;
};
