#include "booking/AvailabilityCalendar.hpp"
#include "booking/BookingPolicy.hpp"
#include "core/exception.hpp"
#include "rooms/floor.hpp"
#include "people/guest.hpp"
#include "hotel/hotel.hpp"
#include "booking/Reservation.hpp"
#include "rooms/standard_room.hpp"

#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace
{
int read_int(const std::string& prompt, int minimum = 0)
{
    while (true)
    {
        std::cout << prompt;
        std::string value;
        std::getline(std::cin, value);

        try
        {
            std::size_t position = 0;
            int result = std::stoi(value, &position);
            if (position == value.size() && result >= minimum)
                return result;
        }
        catch (const std::exception&)
        {
        }

        std::cout << "Введите целое число не меньше " << minimum << ".\n";
    }
}

std::string read_string(const std::string& prompt)
{
    std::cout << prompt;
    std::string value;
    std::getline(std::cin, value);
    return value;
}

Date make_date(int days_from_today)
{
    return now_date() + std::chrono::hours(24 * days_from_today);
}

std::string status_name(ReservationStatus status)
{
    switch (status)
    {
    case ReservationStatus::Pending:
        return "ожидает подтверждения";
    case ReservationStatus::Confirmed:
        return "подтверждена";
    case ReservationStatus::CheckedIn:
        return "гость заселён";
    case ReservationStatus::Cancelled:
        return "отменена";
    case ReservationStatus::NoShow:
        return "гость не приехал";
    case ReservationStatus::Completed:
        return "завершена";
    }
    return "неизвестен";
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

void add_guest(std::vector<std::unique_ptr<Guest>>& guests)
{
    std::cout << "\nИнформация о новом госте\n";
    int number = static_cast<int>(guests.size()) + 1;
    int age = read_int("Возраст: ", 0);
    std::string name = read_string("ФИО: ");
    std::string gender = read_string("Пол: ");
    std::string country = read_string("Страна: ");
    std::string city = read_string("Город: ");
    std::string street = read_string("Улица: ");
    int house = read_int("Номер дома: ", 1);
    int flat = read_int("Номер квартиры (0, если нет): ", 0);

    guests.push_back(std::make_unique<Guest>(
        age, name, gender, number, country, city, street, house, flat));
    std::cout << "Гость добавлен. Его ID: " << number << "\n";
}

void print_guests(const std::vector<std::unique_ptr<Guest>>& guests)
{
    if (guests.empty())
    {
        std::cout << "Гостей пока нет.\n";
        return;
    }

    std::cout << "\nСписок гостей:\n";
    for (std::size_t index = 0; index < guests.size(); ++index)
    {
        const Guest& guest = *guests[index];
        std::cout << "  ID " << index + 1 << ": " << guest.getName()
                  << ", возраст: " << guest.getAge()
                  << ", адрес: " << guest.full_address() << "\n";
    }
}

int select_guest(const std::vector<std::unique_ptr<Guest>>& guests)
{
    print_guests(guests);
    if (guests.empty())
        return -1;

    int id = read_int("Введите ID гостя: ", 1);
    if (id > static_cast<int>(guests.size()))
    {
        std::cout << "Гость с таким ID не найден.\n";
        return -1;
    }
    return id - 1;
}

void print_reservations(
    const std::vector<std::unique_ptr<Reservation>>& reservations)
{
    if (reservations.empty())
    {
        std::cout << "Броней пока нет.\n";
        return;
    }

    std::cout << "\nСписок броней:\n";
    for (const auto& reservation : reservations)
    {
        std::cout << "  " << reservation->id()
                  << ", гость: " << reservation->guest()->getName()
                  << ", номер: " << reservation->room()->get_number()
                  << ", статус: " << status_name(reservation->status())
                  << ", сумма: " << reservation->total_price().value() << "\n";
    }
}
}

int main()
{
    Address address{"Минск", "Центральная", "1"};
    Hotel hotel("Пример", 3, address);
    Floor floor(1, &hotel);
    std::vector<std::unique_ptr<StandardRoom>> rooms;
    for (int number = 101; number <= 108; ++number)
    {
        int capacity = number == 101 ? 1 : (number >= 106 ? 4 : 2);
        rooms.push_back(std::make_unique<StandardRoom>(
            number, &floor, Money(120.0 + (number - 101) * 20),
            RoomStatus::Free, capacity, std::vector<Amenity>(), 24.0, false));
        floor.add_room(rooms.back().get());
    }
    hotel.add_floor(&floor);

    CancellationPolicy policy(2, 50.0);
    AvailabilityCalendar calendar;
    std::vector<std::unique_ptr<Guest>> guests;
    std::vector<std::unique_ptr<Reservation>> reservations;

    std::cout << "Система гостиницы\n";
    add_guest(guests);

    bool running = true;
    while (running && std::cin.good())
    {
        std::cout << "\n1. Добавить гостя\n"
                  << "2. Показать гостей\n"
                  << "3. Показать свободные номера\n"
                  << "4. Создать и подтвердить бронь\n"
                  << "5. Показать статус броней\n"
                  << "6. Отменить бронь\n"
                  << "0. Выход\n";
        int command = read_int("Выберите действие: ", 0);

        try
        {
            if (command == 1)
            {
                add_guest(guests);
            }
            else if (command == 2)
            {
                print_guests(guests);
            }
            else if (command == 3)
            {
                int guests_count = read_int("Количество гостей: ", 1);
                int start_day = read_int("За сколько дней заезд: ", 0);
                int nights = read_int("Количество ночей: ", 1);
                print_rooms(hotel.find_available_rooms(
                    make_date(start_day), make_date(start_day + nights),
                    guests_count));
            }
            else if (command == 4)
            {
                int guest_index = select_guest(guests);
                if (guest_index < 0)
                    continue;
                if (!guests[guest_index]->can_make_reservation())
                {
                    std::cout << "Данные гостя неполные или он несовершеннолетний.\n";
                    continue;
                }

                int guests_count = read_int("Количество гостей в номере: ", 1);
                int start_day = read_int("За сколько дней заезд: ", 0);
                int nights = read_int("Количество ночей: ", 1);
                Date check_in = make_date(start_day);
                Date check_out = make_date(start_day + nights);
                std::vector<Room*> available =
                    hotel.find_available_rooms(check_in, check_out, guests_count);
                print_rooms(available);
                if (available.empty())
                    continue;

                int room_number = read_int("Введите номер выбранной комнаты: ", 1);
                Room* selected_room = nullptr;
                for (Room* room : available)
                    if (room->get_number() == room_number)
                        selected_room = room;
                if (selected_room == nullptr)
                {
                    std::cout << "Этот номер недоступен.\n";
                    continue;
                }

                std::string id = "CLI-" + std::to_string(reservations.size() + 1);
                auto reservation = std::make_unique<Reservation>(
                    id, guests[guest_index].get(), selected_room, check_in,
                    check_out, guests_count, &policy);
                reservation->confirm(calendar);
                std::cout << "Бронь " << id << " подтверждена. Сумма: "
                          << reservation->total_price().value() << "\n";
                reservations.push_back(std::move(reservation));
            }
            else if (command == 5)
            {
                print_reservations(reservations);
            }
            else if (command == 6)
            {
                print_reservations(reservations);
                if (reservations.empty())
                    continue;

                std::string id = read_string("Введите ID брони: ");
                for (const auto& reservation : reservations)
                {
                    if (reservation->id() == id)
                    {
                        reservation->cancel(now_date());
                        std::cout << "Бронь отменена.\n";
                        break;
                    }
                }
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

    return 0;
}
