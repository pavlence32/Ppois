#pragma once

#include <string>

class EnergyMeter
{
    int id_;
    std::string zone_;
    double reading_kwh_;
    double tariff_;

public:
    EnergyMeter(int id, std::string zone, double tariff, double initial_reading = 0.0);

    double cost() const;
    void record(double delta_kwh);
    double consumption_since(double previous_reading) const;
    bool is_peak(double threshold) const;
    void reset_reading();
};
