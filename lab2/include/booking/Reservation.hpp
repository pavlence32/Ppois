#pragma once

#include "core/model.hpp"

#include "booking/reservation_note.hpp"
#include "finance/payment.hpp"

#include <optional>
#include <string>

class Guest;
class Room;
class AvailabilityCalendar;
class BookingPolicy;

class Reservation
{
    friend class Stay;
    std::string id_;
    Guest* guest_;
    Room* room_;
    Date check_in_;
    Date check_out_;
    int guests_count_;
    ReservationStatus status_;
    Money total_price_;
    Date created_at_;
    BookingPolicy* policy_;
    AvailabilityCalendar* calendar_;
    bool deposit_paid_ = false;
    bool breakfast_included_ = false;
    Money deposit_;
    Money service_total_;
    double discount_percent_ = 0.0;
    std::string comment_;
    bool paid_ = false;
    std::optional<Payment> payment_;
    bool invoice_sent_ = false;
    std::string payment_method_;
    std::string source_;
    int changes_count_ = 0;
    std::string confirmation_code_;
    std::string cancellation_reason_;
    std::string invoice_number_;
    ReservationNote arrival_note_;
    ReservationNote departure_note_;

public:
    Reservation(std::string id, Guest* guest, Room* room, Date check_in,
                Date check_out, int guests_count, BookingPolicy* policy = nullptr);
    Reservation(int id, Guest* guest, Room* room, Date check_in, Date check_out,
                int guests_count, BookingPolicy* policy = nullptr);

    const std::string& id() const;
    Guest* guest() const;
    Room* room() const;
    Date check_in() const;
    Date check_out() const;
    int guests_count() const;
    ReservationStatus status() const;
    Money total_price() const;
    Date created_at() const;
    BookingPolicy* policy() const;

    void confirm(AvailabilityCalendar& calendar);
    Money cancel(Date now);
    void extend(Date new_check_out, AvailabilityCalendar& calendar);
    int nights() const;
    Money calculate_total() const;
    bool overlaps(Date start, Date end) const;
    bool is_active() const;
    void mark_no_show(Date now);
    bool can_check_in(Date now) const;
    bool can_cancel(Date now) const;
    void change_guest_count(int guests);
    Money price_per_night() const;
    bool is_finished() const;
    void pay(const std::string& method);
    bool is_paid() const;
    const std::string& payment_method() const;
    Money refund_amount(Date now) const;
};
