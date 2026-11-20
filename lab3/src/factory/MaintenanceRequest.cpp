#include "factory/MaintenanceRequest.hpp"

#include "core/exceptions.hpp"

MaintenanceRequest::MaintenanceRequest(int id,
                                       int machine_id,
                                       std::string description,
                                       Priority priority,
                                       Money estimated_cost)
    : id_(id),
      machine_id_(machine_id),
      description_(std::move(description)),
      priority_(priority),
      completed_(false),
      estimated_cost_(estimated_cost)
{
    if (description_.empty())
        throw FactoryException("Заявка на ремонт должна содержать описание");
}

void MaintenanceRequest::escalate()
{
    if (priority_ == Priority::Low)
        priority_ = Priority::Normal;
    else if (priority_ == Priority::Normal)
        priority_ = Priority::High;
    else
        priority_ = Priority::Critical;
}

Money MaintenanceRequest::complete(const Money& actual_cost)
{
    if (completed_)
        throw InvalidOrderStateException("Заявка уже выполнена");
    if (!actual_cost.is_positive())
        throw InvalidMoneyException("Стоимость ремонта должна быть положительной");
    completed_ = true;
    return actual_cost - estimated_cost_;
}

bool MaintenanceRequest::is_urgent() const
{
    return priority_ == Priority::High || priority_ == Priority::Critical;
}

int MaintenanceRequest::priority_rank() const
{
    switch (priority_)
    {
    case Priority::Low:
        return 1;
    case Priority::Normal:
        return 2;
    case Priority::High:
        return 3;
    case Priority::Critical:
        return 4;
    }
    return 0;
}

bool MaintenanceRequest::is_completed() const
{
    return completed_;
}

int MaintenanceRequest::machine_id() const
{
    return machine_id_;
}
