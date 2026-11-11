#include "booking/AvailabilityCalendar.hpp"

#include "booking/Reservation.hpp"
#include "core/exception.hpp"
#include "rooms/room.hpp"

#include <algorithm>
#include <string>

bool AvailabilityCalendar::is_free(Room* room, Date start, Date end) const
{
    if (room == nullptr || !valid_date_range(start, end))
        return false;
    auto it = bookings_.find(room);
    if (it == bookings_.end())
        return room->get_status() == RoomStatus::Free;
    for (Reservation* reservation : it->second)
        if (reservation->overlaps(start, end))
            return false;
    return room->get_status() != RoomStatus::Dirty;
}

void AvailabilityCalendar::book(Reservation* reservation)
{
    if (reservation == nullptr || !is_free(reservation->room(),
                                            reservation->check_in(),
                                            reservation->check_out()))
        throw OverbookingException{"Номер занят на выбранные даты"};
    bookings_[reservation->room()].push_back(reservation);
}

void AvailabilityCalendar::release(Reservation* reservation)
{
    if (reservation == nullptr)
        throw ReservationNotFoundException{"Бронь не найдена"};
    auto it = bookings_.find(reservation->room());
    if (it == bookings_.end())
        throw ReservationNotFoundException{"Бронь не найдена"};
    auto& list = it->second;
    auto found = std::find(list.begin(), list.end(), reservation);
    if (found == list.end())
        throw ReservationNotFoundException{"Бронь не найдена"};
    list.erase(found);
    if (list.empty())
        bookings_.erase(it);
}

Reservation& AvailabilityCalendar::find_reservation(const std::string& id) const
{
    for (const auto& entry : bookings_)
        for (Reservation* reservation : entry.second)
            if (reservation->id() == id)
                return *reservation;
    throw ReservationNotFoundException{"Бронь не найдена"};
}

Reservation& AvailabilityCalendar::find_reservation(int id) const
{
    return find_reservation(std::to_string(id));
}

std::vector<Room*> AvailabilityCalendar::find_free_rooms(
    const std::vector<Room*>& rooms, Date start, Date end, int guests) const
{
    std::vector<Room*> result;
    for (Room* room : rooms) {
        if (is_free(room, start, end)) {
            try {
                room->can_accommodate(guests);
                result.push_back(room);
            } catch (const RoomOverCapacityException&) {
            }
        }
    }
    return result;
}

double AvailabilityCalendar::get_occupancy(const std::vector<Room*>& rooms,
                                           Date start, Date end) const
{
    if (rooms.empty())
        return 0.0;
    std::size_t occupied = 0;
    for (Room* room : rooms)
        if (!is_free(room, start, end))
            ++occupied;
    return 100.0 * static_cast<double>(occupied) / rooms.size();
}
