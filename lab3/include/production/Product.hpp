#pragma once

#include "core/Money.hpp"

#include <string>
#include <vector>

class ProductSpecification
{
    std::string code_;
    double weight_kg_;
    int labor_minutes_;
    std::vector<std::string> required_materials_;
    double tolerance_mm_;

public:
    ProductSpecification(std::string code,
                         double weight_kg,
                         int labor_minutes,
                         std::vector<std::string> required_materials,
                         double tolerance_mm);

    bool is_within_tolerance(double measured) const;
    double production_hours(int quantity) const;
    bool requires_material(const std::string& material) const;
    bool is_valid() const;
};

class Product
{
    int id_;
    std::string name_;
    ProductSpecification specification_;
    Money price_;
    int stock_;

public:
    Product(int id,
            std::string name,
            ProductSpecification specification,
            Money price,
            int stock = 0);

    bool is_available(int quantity) const;
    Money inventory_value() const;
    void reserve(int quantity);
    void restock(int quantity);
    bool matches_specification(const ProductSpecification& specification) const;
    int stock() const;
    int id() const;
    Money unit_price() const;
    const ProductSpecification& specification() const;
};
