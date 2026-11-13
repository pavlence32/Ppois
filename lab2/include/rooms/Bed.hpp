#pragma once
#include <vector>
#include <string> 
enum class TypeBed{Single,Double,Sofa,Bunk};
class Bed
{
    TypeBed type;
    int capacity;
    bool occupied = false;
    bool extra = false;
    public:
    Bed(TypeBed type,int capacity);
    bool fits_guests(int guests) const;
    bool is_double() const;
    int get_capacity() const;
};