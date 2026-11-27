#include "production/Warehouse.hpp"

#include "core/exceptions.hpp"

#include <algorithm>

InventoryItem::InventoryItem(Material material, std::string location, int quantity)
    : material_(std::move(material)),
      quantity_(quantity),
      location_(std::move(location)),
      reserved_(0)
{
}

int InventoryItem::available() const
{
    return quantity_ - reserved_;
}

void InventoryItem::reserve(int amount)
{
    if (amount <= 0)
        throw InvalidMoneyException("Резерв должен быть положительным");
    if (amount > available())
        throw InsufficientStockException("Недостаточно свободного остатка");
    reserved_ += amount;
}

void InventoryItem::release(int amount)
{
    if (amount > reserved_)
        throw FactoryException("Нельзя освободить больше зарезервированного");
    reserved_ -= amount;
}

bool InventoryItem::below_reorder() const
{
    return quantity_ <= 0 || material_.needs_reorder();
}

void InventoryItem::add(int amount)
{
    if (amount <= 0)
        throw FactoryException("Приход должен быть положительным");
    quantity_ += amount;
}

int InventoryItem::material_id() const
{
    return material_.id();
}

const std::string& InventoryItem::location() const
{
    return location_;
}

int InventoryItem::quantity() const
{
    return quantity_;
}

int InventoryItem::reserved() const
{
    return reserved_;
}

Money InventoryItem::value() const
{
    return material_.valuation(quantity_);
}

Warehouse::Warehouse(int id, std::string name, Address address, int capacity)
    : id_(id), name_(std::move(name)), address_(std::move(address)), capacity_(capacity)
{
    if (capacity <= 0)
        throw CapacityExceededException("Ёмкость склада должна быть положительной");
}

bool Warehouse::can_accept(int units) const
{
    return units > 0 && free_capacity() >= units;
}

void Warehouse::store(InventoryItem item)
{
    if (!can_accept(1))
        throw CapacityExceededException("Склад заполнен");
    items_.push_back(std::move(item));
}

InventoryItem* Warehouse::find(int material_id)
{
    const auto it = std::find_if(items_.begin(), items_.end(),
                                 [material_id](const InventoryItem& item) {
                                     return item.material_id() == material_id;
                                 });
    return it == items_.end() ? nullptr : &*it;
}

Money Warehouse::total_value() const
{
    Money result(0.0);
    for (const auto& item : items_)
        result += item.value();
    return result;
}

int Warehouse::free_capacity() const
{
    return capacity_ - occupancy();
}

void Warehouse::dispatch(int material_id, int quantity)
{
    InventoryItem* item = find(material_id);
    if (item == nullptr)
        throw EntityNotFoundException("Материал отсутствует на складе");
    if (quantity <= 0 || quantity > item->available())
        throw InsufficientStockException("Недостаточно материала для отгрузки");
    item->reserve(quantity);
}

int Warehouse::occupancy() const
{
    return static_cast<int>(items_.size());
}

const std::vector<InventoryItem>& Warehouse::items() const
{
    return items_;
}
