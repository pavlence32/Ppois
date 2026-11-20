#include "core/exceptions.hpp"

FactoryException::FactoryException(std::string message)
    : message_(std::move(message))
{
}

std::string FactoryException::getMessage() const
{
    return message_;
}
