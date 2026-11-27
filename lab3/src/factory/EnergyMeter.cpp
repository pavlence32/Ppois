#include "factory/EnergyMeter.hpp"

#include "core/exceptions.hpp"

EnergyMeter::EnergyMeter(int id, std::string zone, double tariff, double initial_reading)
    : id_(id), zone_(std::move(zone)), reading_kwh_(initial_reading), tariff_(tariff)
{
    if (tariff <= 0.0)
        throw InvalidMoneyException("Тариф на энергию должен быть положительным");
}

double EnergyMeter::cost() const
{
    return reading_kwh_ * tariff_;
}

void EnergyMeter::record(double delta_kwh)
{
    if (delta_kwh < 0.0)
        throw FactoryException("Показание счётчика не может уменьшаться");
    reading_kwh_ += delta_kwh;
}

double EnergyMeter::consumption_since(double previous_reading) const
{
    if (previous_reading < 0.0 || previous_reading > reading_kwh_)
        throw InvalidDateRangeException("Некорректное предыдущее показание счётчика");
    return reading_kwh_ - previous_reading;
}

bool EnergyMeter::is_peak(double threshold) const
{
    return reading_kwh_ >= threshold;
}

void EnergyMeter::reset_reading()
{
    reading_kwh_ = 0.0;
}
