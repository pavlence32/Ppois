#include "production/Product.hpp"

#include "core/exceptions.hpp"

#include <algorithm>
#include <cmath>

ProductSpecification::ProductSpecification(std::string code,
                                           double weight_kg,
                                           int labor_minutes,
                                           std::vector<std::string> required_materials,
                                           double tolerance_mm)
    : code_(std::move(code)),
      weight_kg_(weight_kg),
      labor_minutes_(labor_minutes),
      required_materials_(std::move(required_materials)),
      tolerance_mm_(tolerance_mm)
{
}

bool ProductSpecification::is_within_tolerance(double measured) const
{
    return std::fabs(measured - weight_kg_) <= tolerance_mm_ / 1000.0;
}

double ProductSpecification::production_hours(int quantity) const
{
    if (quantity <= 0)
        return 0.0;
    return static_cast<double>(labor_minutes_ * quantity) / 60.0;
}

bool ProductSpecification::requires_material(const std::string& material) const
{
    return std::find(required_materials_.begin(), required_materials_.end(), material)
        != required_materials_.end();
}

bool ProductSpecification::is_valid() const
{
    return !code_.empty() && weight_kg_ > 0.0 && labor_minutes_ > 0
        && !required_materials_.empty() && tolerance_mm_ > 0.0;
}

Product::Product(int id,
                 std::string name,
                 ProductSpecification specification,
                 Money price,
                 int stock)
    : id_(id),
      name_(std::move(name)),
      specification_(std::move(specification)),
      price_(price),
      stock_(stock)
{
}

bool Product::is_available(int quantity) const
{
    return quantity > 0 && stock_ >= quantity;
}

Money Product::inventory_value() const
{
    return price_ * static_cast<double>(stock_);
}

void Product::reserve(int quantity)
{
    if (quantity <= 0)
        throw InvalidMoneyException("Количество должно быть положительным");
    if (!is_available(quantity))
        throw InsufficientStockException("Недостаточно готовой продукции на складе");
    stock_ -= quantity;
}

void Product::restock(int quantity)
{
    if (quantity <= 0)
        throw FactoryException("Партия пополнения должна быть положительной");
    stock_ += quantity;
}

bool Product::matches_specification(const ProductSpecification& specification) const
{
    return specification.is_valid() && specification.production_hours(1)
        == specification_.production_hours(1);
}

int Product::stock() const
{
    return stock_;
}

int Product::id() const
{
    return id_;
}

Money Product::unit_price() const
{
    return price_;
}

const ProductSpecification& Product::specification() const
{
    return specification_;
}
