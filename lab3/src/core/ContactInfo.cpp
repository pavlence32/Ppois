#include "core/ContactInfo.hpp"

#include "core/exceptions.hpp"

#include <cctype>

ContactInfo::ContactInfo(std::string phone, std::string email, std::string office)
    : phone_(std::move(phone)), email_(std::move(email)), office_(std::move(office))
{
}

bool ContactInfo::has_valid_email() const
{
    const auto at = email_.find('@');
    const auto dot = email_.find('.', at == std::string::npos ? 0 : at);
    return at != std::string::npos && at > 0 && dot != std::string::npos
        && dot > at + 1 && dot + 1 < email_.size();
}

bool ContactInfo::has_valid_phone() const
{
    int digits = 0;
    for (char symbol : phone_)
    {
        if (std::isdigit(static_cast<unsigned char>(symbol)))
            ++digits;
    }
    return digits >= 7;
}

std::string ContactInfo::display() const
{
    return phone_ + " / " + email_ + " / " + office_;
}

void ContactInfo::update_phone(const std::string& phone)
{
    if (phone.size() < 5)
        throw FactoryException("Слишком короткий номер телефона");
    phone_ = phone;
}

std::string ContactInfo::email() const
{
    return email_;
}
