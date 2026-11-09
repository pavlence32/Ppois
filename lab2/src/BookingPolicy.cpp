#include "BookingPolicy.hpp"

#include "Reservation.hpp"
#include "exception.hpp"
#include "room.hpp"

CancellationPolicy::CancellationPolicy(int free_cancel_days, double penalty_percent,
                                       bool first_night_only_on_arrival_day)
    : free_cancel_days_(free_cancel_days),
      penalty_percent_(penalty_percent),
      first_night_only_on_arrival_day_(first_night_only_on_arrival_day)
{
    if (free_cancel_days_ < 0 || penalty_percent_ < 0.0 ||
        penalty_percent_ > 100.0)
        throw InvalidPaymentException{"Некорректные параметры политики отмены"};
}

int CancellationPolicy::days_before_arrival(const Reservation& reservation,
                                            Date now) const
{
    return static_cast<int>(std::chrono::duration_cast<std::chrono::hours>(
        reservation.check_in() - now).count() / 24);
}

bool CancellationPolicy::can_cancel(const Reservation& reservation, Date) const
{
    return reservation.status() == ReservationStatus::Pending ||
           reservation.status() == ReservationStatus::Confirmed;
}

Money CancellationPolicy::calculate_penalty(const Reservation& reservation,
                                            Date now) const
{
    if (!can_cancel(reservation, now))
        throw CancellationNotAllowedException{"Отмена брони невозможна"};
    const int days = days_before_arrival(reservation, now);
    if (days > free_cancel_days_)
        return Money{0.0};
    if (days <= 0) {
        const Money first_night = reservation.room()->calculate_price(1);
        return first_night_only_on_arrival_day_ ? first_night
                                                : reservation.total_price();
    }
    return reservation.total_price() * (penalty_percent_ / 100.0);
}

Money CancellationPolicy::get_refund_amount(const Reservation& reservation,
                                            Date now) const
{
    return reservation.total_price() * 1.0 - calculate_penalty(reservation, now);
}

bool CancellationPolicy::has_penalty(const Reservation& reservation, Date now) const
{
    return calculate_penalty(reservation, now).is_positive();
}

Money CancellationPolicy::calculate_refund(const Reservation& reservation, Date now) const
{
    return get_refund_amount(reservation, now);
}

bool CancellationPolicy::is_free_cancellation_period(
    const Reservation& reservation, Date now) const
{
    return days_before_arrival(reservation, now) > free_cancel_days_;
}
