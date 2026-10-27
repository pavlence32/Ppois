#include "Roomtype.hpp"
RoomType::RoomType(std::string type,float priceMultiplier):type{type},priceMultiplier{priceMultiplier}
{}
std::string RoomType::get_type() const
{
    return type;
}
float RoomType::get_priceMultiplier() const
{
    return priceMultiplier;
}