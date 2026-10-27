#include <vector>
#include <string> 
enum class TypeBed{Single,Double,Sofa,Bunk};
class Bed
{
    TypeBed type;
    int capacity;
    public:
    Bed(TypeBed type,int capacity);
};