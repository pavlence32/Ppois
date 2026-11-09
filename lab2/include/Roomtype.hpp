#pragma once

#include <string>
class RoomType
{
    std::string type;
    float priceMultiplier;
    std::string description;
    int defaultCapacity = 1;
    bool premium = false;
    public:
    RoomType(std::string type,float priceMultiplier);
    std::string get_type() const;
    float get_priceMultiplier() const ;

};