#include "rooms/room_key.hpp"

#include "core/exception.hpp"
#include "rooms/room.hpp"

RoomKey::RoomKey(std::string id, Room* room, Guest* owner,
                 Date valid_from, Date valid_until)
    : id_(id),
      room_(room),
      owner_(owner),
      valid_from_(valid_from),
      valid_until_(valid_until),
      active_(true)
{
    if (!valid_date_range(valid_from_, valid_until_))
        throw InvalidDatesException{"Некорректный срок действия ключа"};
}

bool RoomKey::is_valid(Date now) const
{
    return active_ && valid_from_ <= now && now < valid_until_;
}

void RoomKey::open(Room* room)
{
    if (room != room_)
        throw InvalidKeyException{"Ключ принадлежит другому номеру"};
    if (!is_valid(now_date()))
        throw KeyExpiredException{"Срок действия ключа истёк"};
}

void RoomKey::extend(Date new_until)
{
    if (new_until <= valid_until_)
        throw InvalidDatesException{"Новая дата должна продлевать срок ключа"};
    valid_until_ = new_until;
}

void RoomKey::deactivate()
{
    active_ = false;
}

bool RoomKey::belongs_to(Guest* guest) const
{
    return owner_ == guest;
}

bool RoomKey::opens_room(Room* room) const
{
    return room_ == room && is_valid(now_date());
}

int RoomKey::remaining_days(Date now) const
{
    if (!is_valid(now))
        return 0;
    return days_between(now, valid_until_);
}
