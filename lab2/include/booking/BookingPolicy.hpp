#pragma once

#include "core/model.hpp"

class Reservation;

class BookingPolicy
{
public:
    virtual ~BookingPolicy() = default;
    virtual bool can_cancel(const Reservation& reservation, Date now) const = 0;
    virtual Money calculate_penalty(const Reservation& reservation, Date now) const = 0;
};

class CancellationPolicy : public BookingPolicy
{
    int free_cancel_days_;
    double penalty_percent_;
    bool first_night_only_on_arrival_day_;
    int minimum_stay_days_ = 1;
    int maximum_stay_days_ = 365;
    bool requires_deposit_ = false;

public:
    CancellationPolicy(int free_cancel_days, double penalty_percent,
                       bool first_night_only_on_arrival_day = true);
    bool can_cancel(const Reservation& reservation, Date now) const override;
    Money calculate_penalty(const Reservation& reservation, Date now) const override;
    Money get_refund_amount(const Reservation& reservation, Date now) const;
    int days_before_arrival(const Reservation& reservation, Date now) const;
    bool has_penalty(const Reservation& reservation, Date now) const;
    Money calculate_refund(const Reservation& reservation, Date now) const;
    bool is_free_cancellation_period(const Reservation& reservation, Date now) const;
};