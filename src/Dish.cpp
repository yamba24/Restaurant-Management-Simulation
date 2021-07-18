#include <utility>


#include "../include/Dish.h"

Dish::Dish(int d_id, std::string d_name, int d_price, DishType d_type)
: id(d_id)
,name(std::move(d_name))
,price(d_price)
,type(d_type){}
//default destructor
Dish::~Dish() = default;

int Dish::getId() const {
    return id;
}

std::string Dish::getName() const {
    return name;
}

int Dish::getPrice() const {
    return price;
}

DishType Dish::getType() const {
    return type;
}
//copy constructor
Dish::Dish(const Dish &oth)
:Dish(oth.id,oth.name,oth.price,oth.type){

}

//utility function that determine the type of dish and returns its equivalent string. 
std::string Dish::type2string(DishType dType){

     std::string sType;
     if (dType == BVG)
         sType= "BVG";
     else if (dType == ALC)
         sType= "ALC";
     else if (dType == SPC)
         sType= "SPC";
     else if (dType == VEG)
         sType= "VEG";
    return sType;
}

//utility function that determine the type of dish by its equivalent string.
DishType Dish::findDType(std::string dType) {
    if (dType== "BVG")
        return BVG;
    else if (dType == "ALC")
        return ALC;
    else if (dType == "SPC")
        return SPC;
    else
        return VEG;
}
