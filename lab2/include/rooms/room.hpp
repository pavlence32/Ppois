#pragma once

#include "rooms/amenity.hpp"
#include "core/model.hpp"

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
    std::string category_;
    bool smoking_allowed_ = false;
    bool has_safe_ = false;
    int beds_count_ = 1;
    Money cleaning_price_;
    Date last_cleaned_;
    int floor_number_ = 0;
    bool air_conditioning_ = false;
    bool television_ = false;
    bool wifi_ = true;
    int window_count_ = 1;
    int renovation_year_ = 0;
    bool accessible_ = false;
    std::string room_note_;
    std::string cleaning_status_;
    std::string maintenance_note_;
    std::string view_description_;
    std::string key_location_;

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
    void add_reservation_dates(Date start, Date end);
    void remove_reservation_dates(Date start, Date end);
    bool has_free_capacity(int guests) const;
    Money price_for_guests(int guests, int nights) const;
    int free_beds_count() const;
};
