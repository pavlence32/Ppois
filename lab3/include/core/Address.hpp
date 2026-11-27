#pragma once

#include <string>

class Address
{
    std::string street_;
    std::string city_;
    std::string postal_code_;
    std::string country_;
    std::string building_;

public:
    Address(std::string street = "",
            std::string city = "",
            std::string postal_code = "",
            std::string country = "",
            std::string building = "");

    std::string format() const;
    bool same_city(const Address& other) const;
    bool is_complete() const;
    std::string city() const;
};
