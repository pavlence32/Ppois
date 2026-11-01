#include "exception.hpp"

Exception::Exception(std::string message):message{message}
{}

std::string Exception::getMessage() const
{
    return message;
}
