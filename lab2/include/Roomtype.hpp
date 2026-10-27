#include <string>
class RoomType
{
    std::string type;
    float priceMultiplier;
    public:
    RoomType(std::string type,float priceMultiplier);
    std::string get_type() const;
    float get_priceMultiplier() const ;

};