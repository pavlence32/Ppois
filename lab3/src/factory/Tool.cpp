#include "factory/Tool.hpp"

#include "core/exceptions.hpp"

#include <algorithm>

Tool::Tool(int id, std::string name, std::string condition, int max_hours)
    : id_(id),
      name_(std::move(name)),
      condition_(std::move(condition)),
      usage_hours_(0),
      max_hours_(max_hours)
{
    if (max_hours <= 0)
        throw FactoryException("Ресурс инструмента должен быть положительным");
}

void Tool::use(int hours)
{
    if (!is_available())
        throw MachineUnavailableException("Инструмент " + name_ + " непригоден к работе");
    if (hours <= 0)
        throw FactoryException("Время использования должно быть положительным");
    usage_hours_ += hours;
    if (usage_hours_ > max_hours_)
        condition_ = "broken";
}

void Tool::sharpen()
{
    if (condition_ == "broken")
        throw MachineUnavailableException("Сломанный инструмент нельзя заточить");
    usage_hours_ = std::max(0, usage_hours_ - max_hours_ / 10);
    condition_ = "sharpened";
}

bool Tool::needs_replacement() const
{
    return condition_ == "broken" || wear_percent() >= 100.0;
}

double Tool::wear_percent() const
{
    return 100.0 * static_cast<double>(usage_hours_) / max_hours_;
}

bool Tool::is_available() const
{
    return condition_ != "broken";
}
