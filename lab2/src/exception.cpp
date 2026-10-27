#include "exception.hpp"
EmployeeUnavailableException::EmployeeUnavailableException(std::string message):message{message}
{}
std::string EmployeeUnavailableException::getMessage() const
{
    return message;
}