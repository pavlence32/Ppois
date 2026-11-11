#include "booking/WaitList.hpp"

#include "booking/AvailabilityCalendar.hpp"
#include "booking/BookingPolicy.hpp"
#include "booking/GuestRequest.hpp"
#include "booking/Reservation.hpp"
#include "core/exception.hpp"

#include <algorithm>
#include <string>

WaitList::WaitList(int max_size) : max_size_(max_size)
{
    if (max_size_ < 0)
        throw WaitListFullException{"Некорректный размер листа ожидания"};
}

void WaitList::add(GuestRequest* request)
{
    if (request == nullptr || static_cast<int>(requests_.size()) >= max_size_)
        throw WaitListFullException{"Лист ожидания заполнен"};
    requests_.push_back(request);
}

void WaitList::remove(GuestRequest* request)
{
    auto it = std::find(requests_.begin(), requests_.end(), request);
    if (it != requests_.end())
        requests_.erase(it);
}

int WaitList::position_of(Guest* guest) const
{
    for (std::size_t i = 0; i < requests_.size(); ++i)
        if (requests_[i]->guest() == guest)
            return static_cast<int>(i) + 1;
    return -1;
}

GuestRequest* WaitList::find_matching(Room* room) const
{
    for (GuestRequest* request : requests_)
        if (request->status() == RequestStatus::Open && request->matches(room))
            return request;
    return nullptr;
}

Reservation& WaitList::promote_next(Room* room, AvailabilityCalendar& calendar,
                                     BookingPolicy* policy)
{
    GuestRequest* request = find_matching(room);
    if (request == nullptr)
        throw ReservationNotFoundException{"Подходящая заявка не найдена"};
    auto* reservation = new Reservation(std::to_string(requests_.size()), request->guest(), room,
                                        request->check_in(), request->check_out(),
                                        request->guests_count(), policy);
    reservation->confirm(calendar);
    request->fulfill();
    remove(request);
    return *reservation;
}

void WaitList::remove_expired(Date now)
{
    requests_.erase(std::remove_if(requests_.begin(), requests_.end(),
                                   [now](GuestRequest* request) {
                                       return request->is_expired(now);
                                   }),
                    requests_.end());
}

bool WaitList::is_full() const
{
    return static_cast<int>(requests_.size()) >= max_size_;
}

bool WaitList::contains(Guest* guest) const
{
    return position_of(guest) != -1;
}

int WaitList::size() const
{
    return static_cast<int>(requests_.size());
}
