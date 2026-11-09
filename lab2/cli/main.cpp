#include "AvailabilityCalendar.hpp"
#include "BookingPolicy.hpp"
#include "guest.hpp"
#include "Reservation.hpp"
#include "exception.hpp"
#include "floor.hpp"
#include "hotel.hpp"
#include "standard_room.hpp"

#include <chrono>
#include <iostream>
#include <string>
#include <vector>

namespace
{
Date date_after(int days)
{
    return now_date() + std::chrono::hours(24 * days);
}

void print_rooms(const std::vector<Room*>& rooms)
{
    if (rooms.empty())
    {
        std::cout << "Свободных номеров нет.\n";
        return;
    }

    std::cout << "Свободные номера:\n";
    for (Room* room : rooms)
    {
        std::cout << "  №" << room->get_number()
                  << ", вместимость: " << room->get_capacity()
                  << ", цена за ночь: " << room->calculate_price(1).value()
                  << "\n";
    }
}
}

int main()
{
    Address address{"Минск", "Центральная", "1"};
    Hotel hotel("Пример", 3, address);
    Floor floor(1, &hotel);
    StandardRoom room(101, &floor, Money(120.0), RoomStatus::Free,
                      2, std::vector<Amenity>(), 24.0, false);

    floor.add_room(&room);
    hotel.add_floor(&floor);

    Guest guest(25, "Иван Иванов", "м", 1, "Беларусь",
                "Минск", "Ленина", 10, 5);
    CancellationPolicy policy(2, 50.0);
    AvailabilityCalendar calendar;
    Reservation* reservation = nullptr;

    bool running = true;
    while (running)
    {
        std::cout << "\n Система гостиницы \n"
                  << "1. Показать свободные номера\n"
                  << "2. Создать и подтвердить бронь\n"
                  << "3. Показать статус брони\n"
                  << "0. Выход\n"
                  << "Выберите действие: ";

        int command = -1;
        std::cin >> command;

        try
        {
            if (command == 1)
            {
                print_rooms(hotel.find_available_rooms(
                    date_after(1), date_after(3), 1));
            }
            else if (command == 2)
            {
                if (reservation != nullptr)
                {
                    std::cout << "Бронь уже создана.\n";
                    continue;
                }

                reservation = new Reservation(
                    "CLI-1", &guest, &room, date_after(1),
                    date_after(3), 1, &policy);
                reservation->confirm(calendar);
                std::cout << "Бронь подтверждена. Сумма: "
                          << reservation->total_price().value() << "\n";
            }
            else if (command == 3)
            {
                if (reservation == nullptr)
                    std::cout << "Брони пока нет.\n";
                else
                    std::cout << "Статус брони: "
                              << static_cast<int>(reservation->status())
                              << ", сумма: "
                              << reservation->total_price().value() << "\n";
            }
            else if (command == 0)
            {
                running = false;
            }
            else
            {
                std::cout << "Неизвестная команда.\n";
            }
        }
        catch (const Exception& error)
        {
            std::cout << "Ошибка: " << error.getMessage() << "\n";
        }
    }

    delete reservation;
    return 0;
}
