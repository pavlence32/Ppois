#include "Bed.hpp"
Bed::Bed(TypeBed type,int capacity):type{type},capacity{capacity}
{}

bool Bed::fits_guests(int guests) const
{
    return guests > 0 && guests <= capacity;
}

bool Bed::is_double() const
{
    return type == TypeBed::Double || type == TypeBed::Sofa;
}

int Bed::get_capacity() const
{
    return capacity;
}