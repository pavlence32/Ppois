#pragma once

#include "core/Address.hpp"
#include "core/Money.hpp"
#include "production/Material.hpp"

#include <string>
#include <vector>

class InventoryItem
{
    Material material_;
    int quantity_;
    std::string location_;
    int reserved_;

public:
    InventoryItem(Material material, std::string location, int quantity = 0);

    int available() const;
    void reserve(int amount);
    void release(int amount);
    bool below_reorder() const;
    void add(int amount);
    Money value() const;
    int material_id() const;
    const std::string& location() const;
    int quantity() const;
    int reserved() const;
};

class Warehouse
{
    int id_;
    std::string name_;
    Address address_;
    std::vector<InventoryItem> items_;
    int capacity_;

public:
    Warehouse(int id, std::string name, Address address, int capacity);

    bool can_accept(int units) const;
    void store(InventoryItem item);
    InventoryItem* find(int material_id);
    Money total_value() const;
    int free_capacity() const;
    void dispatch(int material_id, int quantity);
    int occupancy() const;
    const std::vector<InventoryItem>& items() const;
};
