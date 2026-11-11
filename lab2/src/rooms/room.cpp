#include "rooms/room.hpp"

#include "core/exception.hpp"

Room::Room(int number, Floor* floor, Money base_price, RoomStatus status,
           int capacity, std::vector<Amenity> amenities, double area)
    : number_(number),
      floor_(floor),
      base_price_(base_price),
      status_(status),
      capacity_(capacity),
      amenities_(amenities),
      area_(area)
{}

Money Room::base_price() const
{
    return base_price_;
}

int Room::get_number() const
{
    return number_;
}

Floor* Room::get_floor() const
{
    return floor_;
}

RoomStatus Room::get_status() const
{
    return status_;
}

int Room::get_capacity() const
{
    return capacity_;
}

double Room::get_area() const
{
    return area_;
}

void Room::reserve()
{
    if (status_ != RoomStatus::Free)
        throw RoomNotAvailableException{"Номер недоступен для бронирования"};
    status_ = RoomStatus::Reserved;
}

void Room::cancel_reservation()
{
    if (status_ != RoomStatus::Reserved)
        throw RoomNotAvailableException{"Номер не зарезервирован"};
    status_ = RoomStatus::Free;
}

void Room::occupy()
{
    if (status_ == RoomStatus::Dirty)
        throw RoomNotCleanedException{"Номер ещё не убран"};
    if (status_ != RoomStatus::Reserved)
        throw RoomNotAvailableException{"Номер не забронирован"};
    status_ = RoomStatus::Occupied;
}

void Room::release()
{
    if (status_ != RoomStatus::Occupied)
        throw RoomNotAvailableException{"Номер не занят"};
    status_ = RoomStatus::Dirty;
}

void Room::mark_clean()
{
    if (status_ == RoomStatus::Dirty)
        status_ = RoomStatus::Free;
}

bool Room::is_available(Date start, Date end) const
{
    if (!valid_date_range(start, end) || status_ != RoomStatus::Free)
        return false;
    for (const auto& reservation : reservations_)
    {
        if (start < reservation.second && reservation.first < end)
            return false;
    }
    return true;
}

void Room::add_amenity(const Amenity& amenity)
{
    amenities_.push_back(amenity);
}

bool Room::has_amenity(const std::string& name) const
{
    for (const auto& amenity : amenities_)
    {
        if (amenity.get_name() == name && amenity.is_usable())
            return true;
    }
    return false;
}

void Room::can_accommodate(int guests) const
{
    if (guests < 0 || guests > capacity_)
        throw RoomOverCapacityException{"Вместимость номера превышена"};
}

void Room::add_reservation_dates(Date start, Date end)
{
    if (!valid_date_range(start, end))
        throw InvalidDatesException{"Некорректные даты брони"};
    reservations_.push_back(std::make_pair(start, end));
}

void Room::remove_reservation_dates(Date start, Date end)
{
    for (auto it = reservations_.begin(); it != reservations_.end(); ++it)
    {
        if (it->first == start && it->second == end)
        {
            reservations_.erase(it);
            return;
        }
    }
}

bool Room::has_free_capacity(int guests) const
{
    return guests > 0 && guests <= capacity_ &&
           status_ != RoomStatus::Dirty;
}

Money Room::price_for_guests(int guests, int nights) const
{
    can_accommodate(guests);
    if (nights <= 0)
        throw InvalidDatesException{"Количество ночей должно быть положительным"};
    return calculate_price(nights);
}

int Room::free_beds_count() const
{
    return capacity_;
}
