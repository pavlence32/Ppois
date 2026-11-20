#pragma once

#include <string>

class ContactInfo
{
    std::string phone_;
    std::string email_;
    std::string office_;

public:
    ContactInfo(std::string phone = "",
                std::string email = "",
                std::string office = "");

    bool has_valid_email() const;
    bool has_valid_phone() const;
    std::string display() const;
    void update_phone(const std::string& phone);
    std::string email() const;
};
