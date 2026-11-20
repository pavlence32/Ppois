#pragma once

#include <string>

class FactoryException
{
    std::string message_;

public:
    explicit FactoryException(std::string message);
    virtual ~FactoryException() = default;

    virtual std::string getMessage() const;
};

class InvalidMoneyException : public FactoryException
{
public:
    using FactoryException::FactoryException;
};

class InvalidDateRangeException : public FactoryException
{
public:
    using FactoryException::FactoryException;
};

class EntityNotFoundException : public FactoryException
{
public:
    using FactoryException::FactoryException;
};

class DuplicateEntityException : public FactoryException
{
public:
    using FactoryException::FactoryException;
};

class UnauthorizedAccessException : public FactoryException
{
public:
    using FactoryException::FactoryException;
};

class InsufficientStockException : public FactoryException
{
public:
    using FactoryException::FactoryException;
};

class MachineUnavailableException : public FactoryException
{
public:
    using FactoryException::FactoryException;
};

class QualityCheckFailedException : public FactoryException
{
public:
    using FactoryException::FactoryException;
};

class InvalidOrderStateException : public FactoryException
{
public:
    using FactoryException::FactoryException;
};

class CapacityExceededException : public FactoryException
{
public:
    using FactoryException::FactoryException;
};

class InvalidShiftException : public FactoryException
{
public:
    using FactoryException::FactoryException;
};

class SafetyViolationException : public FactoryException
{
public:
    using FactoryException::FactoryException;
};
