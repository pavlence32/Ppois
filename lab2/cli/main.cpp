#include "booking/AvailabilityCalendar.hpp"
#include "booking/BookingPolicy.hpp"
#include "core/exception.hpp"
#include "rooms/floor.hpp"
#include "people/guest.hpp"
#include "hotel/hotel.hpp"
#include "booking/Reservation.hpp"
#include "booking/Stay.hpp"
#include "services/breakfast_order.hpp"
#include "services/laundry_order.hpp"
#include "services/spa_booking.hpp"
#include "services/transport_request.hpp"
#include "rooms/standard_room.hpp"

#include <chrono>
#include <iostream>
#include <map>
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

std::vector<Amenity> make_amenities(int number)
{
    std::vector<Amenity> amenities;
    amenities.emplace_back("Wi-Fi", AmenityCategory::Technology, false);
    amenities.emplace_back("Телевизор", AmenityCategory::Technology, false);
    amenities.emplace_back("Кондиционер", AmenityCategory::Comfort, false);

    switch (number % 3)
    {
    case 0:
        amenities.emplace_back("Вид на море", AmenityCategory::Comfort, false);
        break;
    case 1:
        amenities.emplace_back("Вид на сад", AmenityCategory::Comfort, false);
        break;
    default:
        amenities.emplace_back("Вид на город", AmenityCategory::Comfort, false);
        break;
    }

    const int last_digit = number % 10;
    if (last_digit == 1)
        amenities.emplace_back("Сейф", AmenityCategory::Comfort, true, Money(3.0));
    if (last_digit == 2 || last_digit == 7)
        amenities.emplace_back("Балкон", AmenityCategory::Comfort, false);
    if (last_digit == 3 || last_digit >= 6)
        amenities.emplace_back("Мини-бар", AmenityCategory::Food, true, Money(5.0));
    if (last_digit >= 6)
        amenities.emplace_back("Кухня", AmenityCategory::Comfort, false);
    if (last_digit == 5)
        amenities.emplace_back("Рабочий стол", AmenityCategory::Comfort, false);
    if (number % 2 == 0)
        amenities.emplace_back("Фен", AmenityCategory::Comfort, false);

    return amenities;
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

        const std::vector<Amenity>& amenities = room->get_amenities();
        if (amenities.empty())
        {
            std::cout << "      удобства: нет\n";
            continue;
        }

        std::cout << "      удобства: ";
        for (std::size_t index = 0; index < amenities.size(); ++index)
        {
            const Amenity& amenity = amenities[index];
            if (index != 0)
                std::cout << ", ";
            std::cout << amenity.get_name();
            if (!amenity.is_usable())
                std::cout << " (сломан)";
            else if (amenity.is_paid())
                std::cout << " (+" << amenity.get_cost(1).value() << "/ночь)";
        }
        std::cout << "\n";
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
                  << ", сумма: " << reservation->total_price().value()
                  << ", оплата: "
                  << (reservation->is_paid() ? "оплачено" : "не оплачено")
                  << "\n";
    }
}

void print_stays(const std::vector<std::unique_ptr<Stay>>& stays)
{
    bool found = false;
    for (const auto& stay : stays)
    {
        if (stay->status() != StayStatus::Active ||
            stay->reservation()->status() != ReservationStatus::CheckedIn)
            continue;

        found = true;
        std::cout << "  " << stay->reservation()->id()
                  << ", гость: " << stay->reservation()->guest()->getName()
                  << ", номер: " << stay->reservation()->room()->get_number()
                  << "\n";
    }

    if (!found)
        std::cout << "Активных проживаний нет.\n";
}

void print_room_amenities_with_state(Room& room)
{
    std::cout << "Удобства номера №" << room.get_number() << ":\n";
    for (const Amenity& amenity : room.get_amenities())
        std::cout << "  - " << amenity.get_name()
                  << (amenity.is_usable() ? " (исправно)" : " (сломан)")
                  << "\n";
}

void order_service(Stay& stay,
                   std::map<std::string, Money>& service_totals)
{
    const std::string reservation_id = stay.reservation()->id();
    std::cout << "Дополнительные услуги:\n"
              << "  1. Завтрак — 20 за гостя\n"
              << "  2. Прачечная — 10 за вещь\n"
              << "  3. SPA — 50 за процедуру\n"
              << "  4. Трансфер — 30 за гостя\n";
    const int choice = read_int("Выберите услугу: ", 1);
    Money price;

    switch (choice)
    {
    case 1:
    {
        const int guests = read_int("Количество завтраков: ", 1);
        BreakfastOrder order(guests, Money(20));
        price = order.total();
        break;
    }
    case 2:
    {
        const int items = read_int("Количество вещей: ", 1);
        LaundryOrder order(items, Money(10));
        price = order.total();
        break;
    }
    case 3:
    {
        const std::string procedure = read_string("Название процедуры: ");
        SpaBooking order(procedure, Money(50));
        price = order.price();
        break;
    }
    case 4:
    {
        const int guests = read_int("Количество пассажиров: ", 1);
        TransportRequest order(guests, Money(30));
        price = order.price();
        break;
    }
    default:
        std::cout << "Неверный выбор услуги.\n";
        return;
    }

    service_totals[reservation_id] = service_totals[reservation_id] + price;
    std::cout << "Услуга добавлена к счёту. Стоимость: " << price.value()
              << ", всего за услуги: " << service_totals[reservation_id].value()
              << "\n";
}
}

int main()
{
    Address address{"Минск", "Центральная", "1"};
    Hotel hotel("Пример", 4, address);

    const int floors_count = 5;
    const int rooms_per_floor = 10;
    std::vector<std::unique_ptr<Floor>> floors;
    std::vector<std::unique_ptr<StandardRoom>> rooms;

    for (int floor_number = 1; floor_number <= floors_count; ++floor_number)
    {
        floors.push_back(std::make_unique<Floor>(floor_number, &hotel));
        Floor* floor = floors.back().get();

        for (int index = 1; index <= rooms_per_floor; ++index)
        {
            const int number = floor_number * 100 + index;
            const int last_digit = number % 10;
            const int capacity = last_digit == 1 ? 1 : (last_digit >= 6 ? 4 : 2);
            const double price = 100.0 + floor_number * 10.0 + (index - 1) * 15.0;

            rooms.push_back(std::make_unique<StandardRoom>(
                number, floor, Money(price), RoomStatus::Free, capacity,
                make_amenities(number), 20.0 + capacity * 4.0, false));
            floor->add_room(rooms.back().get());
        }

        hotel.add_floor(floor);
    }

    CancellationPolicy policy(2, 50.0);
    AvailabilityCalendar calendar;
    std::vector<std::unique_ptr<Guest>> guests;
    std::vector<std::unique_ptr<Reservation>> reservations;
    std::vector<std::unique_ptr<Stay>> stays;
    std::map<std::string, Money> service_totals;

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
                  << "7. Заселить гостя (check-in)\n"
                  << "8. Выселить гостя (check-out)\n"
                  << "9. Убрать грязные номера\n"
                  << "10. Сломать/починить удобство\n"
                  << "11. Заказать дополнительную услугу\n"
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

                std::cout << "Способ оплаты:\n"
                          << "  1. Банковская карта\n"
                          << "  2. Наличные\n";
                const int payment_choice = read_int("Выберите способ оплаты: ", 1);
                if (payment_choice != 1 && payment_choice != 2)
                {
                    std::cout << "Неверный способ оплаты.\n";
                    continue;
                }

                std::string id = "CLI-" + std::to_string(reservations.size() + 1);
                auto reservation = std::make_unique<Reservation>(
                    id, guests[guest_index].get(), selected_room, check_in,
                    check_out, guests_count, &policy);
                reservation->confirm(calendar);
                reservation->pay(payment_choice == 1 ? "card" : "cash");
                std::cout << "Бронь " << id
                          << " подтверждена. Оплата: оплачено. Сумма: "
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
                        const Date cancellation_date = now_date();
                        const Money refund =
                            reservation->refund_amount(cancellation_date);
                        reservation->cancel(cancellation_date);
                        std::cout << "Бронь отменена.";
                        if (reservation->is_paid())
                            std::cout << " Возврат: " << refund.value();
                        std::cout << "\n";
                        break;
                    }
                }
            }
            else if (command == 7)
            {
                print_reservations(reservations);
                if (reservations.empty())
                    continue;

                std::string id = read_string("Введите ID брони для заселения: ");
                Reservation* target = nullptr;
                for (const auto& reservation : reservations)
                    if (reservation->id() == id)
                        target = reservation.get();

                if (target == nullptr ||
                    target->status() != ReservationStatus::Confirmed)
                {
                    std::cout << "Подтверждённая бронь с таким ID не найдена.\n";
                    continue;
                }

                stays.push_back(std::make_unique<Stay>(target));
                stays.back()->check_in(now_date());
                std::cout << "Гость " << target->guest()->getName()
                          << " заселён в номер "
                          << target->room()->get_number() << ".\n";
            }
            else if (command == 8)
            {
                std::cout << "\nАктивные проживания:\n";
                print_stays(stays);

                std::string id = read_string("Введите ID брони для выселения: ");
                Stay* target = nullptr;
                for (const auto& stay : stays)
                    if (stay->reservation()->id() == id &&
                        stay->status() == StayStatus::Active &&
                        stay->reservation()->status() ==
                            ReservationStatus::CheckedIn)
                        target = stay.get();

                if (target == nullptr)
                {
                    std::cout << "Активное проживание с таким ID не найдено.\n";
                    continue;
                }

                const int room_number = target->reservation()->room()->get_number();
                const Money extra_services = service_totals[target->reservation()->id()];
                target->check_out(now_date());
                std::cout << "Гость выселен. Номер " << room_number
                          << " переведён в статус «грязный».\n"
                          << "Дополнительные услуги: " << extra_services.value()
                          << ". Доплата за услуги при выселении: "
                          << extra_services.value()
                          << "\n";
            }
            else if (command == 9)
            {
                int dirty_count = 0;
                for (const auto& room : rooms)
                    if (room->get_status() == RoomStatus::Dirty)
                        ++dirty_count;

                hotel.clean_all_dirty_rooms();
                std::cout << "Убрано номеров: " << dirty_count << ".\n";
            }
            else if (command == 10)
            {
                int action = read_int(
                    "1 - сломать удобство, 2 - починить удобство: ", 1);
                if (action != 1 && action != 2)
                {
                    std::cout << "Неверный выбор.\n";
                    continue;
                }

                int room_number = read_int("Введите номер комнаты: ", 1);
                Room& room = hotel.find_room(room_number);
                print_room_amenities_with_state(room);

                std::string name = read_string("Введите название удобства: ");
                bool changed = action == 1 ? room.break_amenity(name)
                                           : room.repair_amenity(name);
                if (!changed)
                {
                    std::cout << "Удобство с таким названием не найдено.\n";
                    continue;
                }

                std::cout << (action == 1
                                  ? "Удобство отмечено как сломанное.\n"
                                  : "Удобство отремонтировано.\n");
            }
            else if (command == 11)
            {
                std::cout << "\nАктивные проживания:\n";
                print_stays(stays);

                std::string id = read_string(
                    "Введите ID брони для заказа услуги: ");
                Stay* target = nullptr;
                for (const auto& stay : stays)
                    if (stay->reservation()->id() == id &&
                        stay->status() == StayStatus::Active &&
                        stay->reservation()->status() ==
                            ReservationStatus::CheckedIn)
                        target = stay.get();

                if (target == nullptr)
                {
                    std::cout << "Активное проживание с таким ID не найдено.\n";
                    continue;
                }

                order_service(*target, service_totals);
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
