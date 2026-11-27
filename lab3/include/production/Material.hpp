#pragma once

#include "core/Address.hpp"
#include "core/ContactInfo.hpp"
#include "core/Money.hpp"

#include <string>

class Material
{
    int id_;
    std::string name_;
    std::string unit_;
    int quantity_;
    Money unit_cost_;
    int reorder_level_;

public:
    Material(int id,
             std::string name,
             std::string unit,
             int quantity,
             Money unit_cost,
             int reorder_level);

    void consume(int amount);
    void restock(int amount);
    Money total_value() const;
    bool needs_reorder() const;
    int shortage(int required) const;
    Money valuation(int units) const;
    int id() const;
    int quantity() const;
    const std::string& name() const;
    const std::string& unit() const;
    int reorder_level() const;
};

class Supplier
{
    int id_;
    std::string name_;
    Address address_;
    ContactInfo contact_;
    int lead_days_;
    double rating_;

public:
    Supplier(int id,
             std::string name,
             Address address,
             ContactInfo contact,
             int lead_days,
             double rating);

    bool can_deliver_within(int days) const;
    void adjust_rating(double delta);
    bool is_reliable() const;
    Money quote(const Material& material, int quantity) const;
    bool serves_city(const std::string& city) const;
};
