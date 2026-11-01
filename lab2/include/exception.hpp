#pragma once

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

class InvalidDatesException:public Exception
{
    public:
    using Exception::Exception;
};

class RoomNotAvailableException:public Exception
{
    public:
    using Exception::Exception;
};

class RoomNotCleanedException:public Exception
{
    public:
    using Exception::Exception;
};

class RoomNotFoundException:public Exception
{
    public:
    using Exception::Exception;
};

class RoomOverCapacityException:public Exception
{
    public:
    using Exception::Exception;
};

class DuplicateRoomException:public Exception
{
    public:
    using Exception::Exception;
};

class InvalidKeyException:public Exception
{
    public:
    using Exception::Exception;
};

class KeyExpiredException:public Exception
{
    public:
    using Exception::Exception;
};
class InvalidDateException:public Exception
{
    public:
    using Exception::Exception;
};
class InvalidPaymentException:public Exception
{
    public:
    using Exception::Exception;
};
class InsufficientFundsException:public Exception
{
    public:
    using Exception::Exception;
};

class OverbookingException:public Exception { public: using Exception::Exception; };
class InvalidReservationStateException:public Exception { public: using Exception::Exception; };
class CancellationNotAllowedException:public Exception { public: using Exception::Exception; };
class ReservationNotFoundException:public Exception { public: using Exception::Exception; };
class WaitListFullException:public Exception { public: using Exception::Exception; };