#include "production/Material.hpp"

#include "core/exceptions.hpp"

#include <algorithm>

Material::Material(int id,
                   std::string name,
                   std::string unit,
                   int quantity,
                   Money unit_cost,
                   int reorder_level)
    : id_(id),
      name_(std::move(name)),
      unit_(std::move(unit)),
      quantity_(quantity),
      unit_cost_(unit_cost),
      reorder_level_(reorder_level)
{
}

void Material::consume(int amount)
{
    if (amount <= 0)
        throw FactoryException("Списание должно быть положительным");
    if (amount > quantity_)
        throw InsufficientStockException("Недостаточно материала " + name_);
    quantity_ -= amount;
}

void Material::restock(int amount)
{
    if (amount <= 0)
        throw FactoryException("Приход должен быть положительным");
    quantity_ += amount;
}

Money Material::total_value() const
{
    return unit_cost_ * static_cast<double>(quantity_);
}

bool Material::needs_reorder() const
{
    return quantity_ <= reorder_level_;
}

int Material::shortage(int required) const
{
    return std::max(0, required - quantity_);
}

Money Material::valuation(int units) const
{
    if (units < 0)
        throw InvalidMoneyException("Количество не может быть отрицательным");
    return unit_cost_ * static_cast<double>(units);
}

int Material::id() const
{
    return id_;
}

int Material::quantity() const
{
    return quantity_;
}

Supplier::Supplier(int id,
                   std::string name,
                   Address address,
                   ContactInfo contact,
                   int lead_days,
                   double rating)
    : id_(id),
      name_(std::move(name)),
      address_(std::move(address)),
      contact_(std::move(contact)),
      lead_days_(lead_days),
      rating_(rating)
{
}

bool Supplier::can_deliver_within(int days) const
{
    return lead_days_ <= days;
}

void Supplier::adjust_rating(double delta)
{
    rating_ = std::clamp(rating_ + delta, 0.0, 10.0);
}

bool Supplier::is_reliable() const
{
    return rating_ >= 7.0 && lead_days_ <= 14;
}

Money Supplier::quote(const Material& material, int quantity) const
{
    if (quantity <= 0)
        throw InvalidMoneyException("Количество в заявке должно быть положительным");
    const double premium = rating_ >= 8.0 ? 1.05 : 1.0;
    return material.valuation(quantity) * premium;
}

bool Supplier::serves_city(const std::string& city) const
{
    return address_.city() == city;
}
