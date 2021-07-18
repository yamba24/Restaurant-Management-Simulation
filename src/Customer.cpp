
#include "../include/Customer.h"
#include <iostream>
#include <string>
#include <vector>
#include <climits>

using namespace std;

/*------------------ customer constructor  ------------------*/
Customer::Customer(std::string c_name, int c_id) : name(c_name), id(c_id) {
}

Customer::~Customer() = default; // empty destructor
/*------------------ functions  ------------------*/
int Customer::getId() const {
    return this->id;
}

std::string Customer::getName() const {
    return this->name;
}

/*------------------ vegetarian customer  ------------------*/
VegetarianCustomer::VegetarianCustomer(std::string name, int id) : Customer(name, id) {
}

/*------------------ functions ------------------*/


std::string VegetarianCustomer::toString() const {
    std::string s;
    s = this->getName() + ",veg";
    return s;
}

std::vector<int> VegetarianCustomer::order(const std::vector<Dish> &menu) {
    std::vector<int> vegDish;

    for (unsigned int i = 0; i < menu.size(); i++) {
        if (menu[i].getType() == VEG) {
            vegDish.push_back(menu[i].getId());
            break;
        }
    }

    std::vector<Dish> bvg;
    for (unsigned int k = 0; k < menu.size(); k++) {
        if (menu[k].getType() == BVG) {
            bvg.push_back(menu[k]);
        }
    }
    int max, id;
    max = INT_MIN;
    for (unsigned int l = 0; l < bvg.size(); l++) {
        if (bvg[l].getPrice() > max) {
            max = bvg[l].getPrice();
            id = bvg[l].getId();
        }
    }
    if (max != INT_MIN)
        vegDish.push_back(id);
    if (vegDish.size() != 2) {
        vegDish.clear();
    }
    return vegDish;
}

Customer *VegetarianCustomer::clone() {
    return new VegetarianCustomer(getName(), getId());
}

VegetarianCustomer::~VegetarianCustomer() {

}


/*------------------ cheap customer  ------------------*/
CheapCustomer::CheapCustomer(std::string name, int id) : Customer(name, id), OrderOnce(false) {}

/*------------------ functions ------------------*/
bool CheapCustomer::alreadyOrder() {
    return this->OrderOnce;
}

std::string CheapCustomer::toString() const {
    std::string s;
    s = this->getName() + ",chp";
    return s;
}

std::vector<int> CheapCustomer::order(const std::vector<Dish> &menu) {
    std::vector<int> cheap;
    if (!(alreadyOrder())) { // if the customer hasn't oredered yet than make the order
        int min, id;
        min = INT_MAX;
        for (unsigned int i = 0; i < menu.size(); i++) {
            if (menu[i].getPrice() < min) {
                min = menu[i].getPrice();
                id = i;
            }
        }
        cheap.push_back(menu[id].getId());
        OrderOnce = true;
    }
    return cheap;
}

Customer *CheapCustomer::clone() {
    CheapCustomer *customer = new CheapCustomer(getName(), getId());
    customer->OrderOnce = OrderOnce;
    return customer;
}

CheapCustomer::~CheapCustomer() {

}

/*------------------ spicy customer  ------------------*/
SpicyCustomer::SpicyCustomer(std::string name, int id) : Customer(name, id), counter(0) {}

/*------------------ functions ------------------*/
std::string SpicyCustomer::toString() const {
    std::string s;
    s = this->getName() + ",spc";
    return s;
}

std::vector<int> SpicyCustomer::order(const std::vector<Dish> &menu) {
    std::vector<int> spicy;
    std::vector<Dish> spicy1;
    std::vector<Dish> bvg;

    if (counter == 0) {
        for (unsigned int i = 0; i < menu.size(); i++) { // find all the spc dishes in the menu
            if (menu[i].getType() == SPC) {
                spicy1.push_back(menu[i]);
            }
        }
        int max, id(-1);
        max = INT_MIN;
        for (unsigned int j = 0; j < spicy1.size(); j++) {
            if (spicy1[j].getPrice() > max) {
                max = spicy1[j].getPrice();
                id = spicy1[j].getId();
            }
        }
        if(id!=-1)
            spicy.push_back(id);
        counter++;
    } else if (counter > 0) {
        for (unsigned int l = 0; l < menu.size(); l++) { // find the bvg drink
            if (menu[l].getType() == BVG) {
                bvg.push_back(menu[l]);
            }
        }
        int min, id(-1);
        min = INT_MAX;
        for (unsigned int k = 0; k < bvg.size(); k++) {
            if (bvg[k].getPrice() < min) {
                min = bvg[k].getPrice();
                id = bvg[k].getId();
            }

        }
        if(id!=-1)
            spicy.push_back(id);
        counter++;
    }
    return spicy;
}


Customer *SpicyCustomer::clone() {
    SpicyCustomer *customer = new SpicyCustomer(getName(), getId());
    customer->counter = this->counter;
    return customer;
}

SpicyCustomer::~SpicyCustomer() {

}

/*------------------ alcoholic customer  ------------------*/
AlchoholicCustomer::AlchoholicCustomer(std::string name, int id) : Customer(name, id), counter(0), alc() {}

/*------------------ functions ------------------*/
std::string AlchoholicCustomer::toString() const {
    std::string s;
    s = this->getName() + ",alc";
    return s;
}

std::vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu) {
    std::vector<int> ans;
    if (this->counter == 0) { // if the customer does not order yet, than find all the alcoholic drinks
        for (unsigned int i = 0; i < menu.size(); i++) {
            if (menu[i].getType() == ALC) {
                this->alc.push_back(menu[i]);
            }
        }
    }
    if (alc.size() > 0) {
        int min, id;
        min = INT_MAX;
        for (unsigned int j = 0; j < alc.size(); j++) {
            if (alc[j].getPrice() < min) {
                min = alc[j].getPrice();
                id = j;
            }
        }
        ans.push_back(alc[id].getId());
        std::vector<Dish> temp;
        for (unsigned int k = 0; k < alc.size(); k++) {
            if ((int)k != id) {
                temp.push_back(alc[k]);
            }
        }
        alc.clear();
        for (unsigned int l = 0; l < temp.size(); l++) {
            alc.push_back(temp[l]);
        }
        counter++;
    }
    return ans;
}

Customer *AlchoholicCustomer::clone() {
    AlchoholicCustomer *customer = new AlchoholicCustomer(getName(), getId());
    customer->counter = counter;
    for (Dish dish : alc)
        customer->alc.push_back(dish);
    return customer;
}

AlchoholicCustomer::~AlchoholicCustomer() {

}
//

