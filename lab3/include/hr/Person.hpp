#pragma once

#include "core/Address.hpp"
#include "core/ContactInfo.hpp"

#include <string>

class Person
{
    std::string full_name_;
    Address address_;
    ContactInfo contact_;
    std::string tax_id_;
    bool active_;

public:
    Person(std::string full_name,
           Address address,
           ContactInfo contact,
           std::string tax_id,
           bool active = true);

    void update_contact(const ContactInfo& contact);
    void relocate(const Address& address);
    bool has_complete_profile() const;
    std::string initials() const;
    void deactivate();

    const std::string& full_name() const;
    const Address& address() const;
    const ContactInfo& contact() const;
    bool is_active() const;
};
