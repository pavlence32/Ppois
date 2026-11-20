#include "factory/Report.hpp"

#include "core/exceptions.hpp"

#include <sstream>

Report::Report(std::string title,
               DatePeriod period,
               Money revenue,
               Money expenses,
               std::string author)
    : title_(std::move(title)),
      period_(period),
      revenue_(revenue),
      expenses_(expenses),
      author_(std::move(author))
{
    if (!period_.is_valid())
        throw InvalidDateRangeException("Отчётный период задан неверно");
    if (!revenue_.is_positive())
        throw InvalidMoneyException("Выручка должна быть положительной");
}

Money Report::profit() const
{
    return revenue_ - expenses_;
}

double Report::margin() const
{
    if (revenue_.is_zero())
        return 0.0;
    return 100.0 * profit().value() / revenue_.value();
}

bool Report::is_profitable() const
{
    return profit().is_positive();
}

std::string Report::summary() const
{
    std::ostringstream stream;
    stream << title_ << " (" << author_ << "), дней: " << period_.duration_days()
           << ", прибыль: " << profit().value() << ", маржа: " << margin() << "%";
    return stream.str();
}
