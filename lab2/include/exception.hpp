#include <string>
class Exception
{
    std::string message;
    public:
    Exception(std::string message);
    virtual std::string getMessage() const;

};
class EmployeeUnavailableException:public Exception
{
    public:
    using Exception::Exception;
};