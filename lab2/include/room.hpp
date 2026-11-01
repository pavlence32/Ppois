#pragma once

#include "amenity.hpp"
#include "model.hpp"

#include <string>
#include <vector>

class Floor;

class Room
{
    int number_;
    Floor* floor_;
    Money base_price_;
    RoomStatus status_;
    int capacity_;
    std::vector<Amenity> amenities_;
    double area_;
    std::vector<std::pair<Date, Date>> reservations_;

protected:
    Money base_price() const;

public:
    Room(int number, Floor* floor, Money base_price, RoomStatus status,
         int capacity, std::vector<Amenity> amenities, double area);
    virtual ~Room() = default;

    int get_number() const;
    Floor* get_floor() const;
    RoomStatus get_status() const;
    int get_capacity() const;
    double get_area() const;
    void reserve();
    void cancel_reservation();
    void occupy();
    void release();
    void mark_clean();
    bool is_available(Date start, Date end) const;
    void add_amenity(const Amenity& amenity);
    bool has_amenity(const std::string& name) const;
    void can_accommodate(int guests) const;
    virtual Money calculate_price(int nights) const = 0;
};
