#pragma once

#include "model.hpp"

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

public:
    CancellationPolicy(int free_cancel_days, double penalty_percent,
                       bool first_night_only_on_arrival_day = true);
    bool can_cancel(const Reservation& reservation, Date now) const override;
    Money calculate_penalty(const Reservation& reservation, Date now) const override;
    Money get_refund_amount(const Reservation& reservation, Date now) const;
    int days_before_arrival(const Reservation& reservation, Date now) const;
};