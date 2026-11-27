#include "hr/Person.hpp"

#include "core/exceptions.hpp"

#include <sstream>

Person::Person(std::string full_name,
               Address address,
               ContactInfo contact,
               std::string tax_id,
               bool active)
    : full_name_(std::move(full_name)),
      address_(std::move(address)),
      contact_(std::move(contact)),
      tax_id_(std::move(tax_id)),
      active_(active)
{
}

void Person::update_contact(const ContactInfo& contact)
{
    if (!contact.has_valid_email())
        throw UnauthorizedAccessException("Некорректный email при обновлении контактов");
    contact_ = contact;
}

void Person::relocate(const Address& address)
{
    if (!address.is_complete())
        throw FactoryException("Неполный адрес");
    address_ = address;
}

bool Person::has_complete_profile() const
{
    return !full_name_.empty() && !tax_id_.empty() && address_.is_complete()
        && contact_.has_valid_email();
}

std::string Person::initials() const
{
    std::istringstream stream(full_name_);
    std::string word;
    std::string result;
    while (stream >> word)
        result += word.front();
    return result;
}

void Person::deactivate()
{
    active_ = false;
}

const std::string& Person::full_name() const
{
    return full_name_;
}

const Address& Person::address() const
{
    return address_;
}

const ContactInfo& Person::contact() const
{
    return contact_;
}

bool Person::is_active() const
{
    return active_;
}
