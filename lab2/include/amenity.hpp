#pragma once

#include "model.hpp"

class Amenity
{
    std::string name_;
    AmenityCategory category_;
    bool paid_;
    Money price_per_day_;
    bool usable_;
    int repair_count_ = 0;
    int warranty_months_ = 0;
    std::string provider_;

public:
    Amenity(std::string name, AmenityCategory category, bool paid,
            Money price_per_day = Money{});
    const std::string& get_name() const;
    Money get_cost(int nights) const;
    void mark_broken();
    void repair();
    bool is_usable() const;
    bool is_paid() const;
    void change_price(Money price);
    bool is_available_for(int nights) const;
};
