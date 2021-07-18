#include <utility>
#include <fstream>
#include <sstream>
#include "../include/Restaurant.h"
#include <algorithm>
using namespace std;
//empty constructor
Restaurant::Restaurant() :open{false}, tables(std::vector<Table*>()), menu(std::vector<Dish>()), actionsLog(std::vector<BaseAction*>()), customer_id(0), numoftables(0){
}
//condtructor
Restaurant::Restaurant(const std::string &configFilePath): open{false}, tables(std::vector<Table*>()), menu(std::vector<Dish>()), actionsLog(std::vector<BaseAction*>()), customer_id(0), numoftables(0) {
    std::ifstream myfile(configFilePath);
    std::string line;
    std::vector<string> allrelevant;

    while (getline(myfile, line)) { // transfers the lines we wish to read into to the vector
        if (line[0] != '#' && line != "") {
            allrelevant.push_back(line);
        }
    }
    numoftables = stoi(allrelevant[0]); // initialize the number of tables
    std::vector<string> capacity; // initialize the capacity of each table
    capacity = split(allrelevant[1], ',');
    for (unsigned int i = 0; i < capacity.size(); i++) {
        Table *t = new Table(stoi(capacity[i]));
        tables.push_back(t);
    }
    int id = 0;
    for (unsigned int i = 2; i < allrelevant.size(); i++) {
        std::vector<string> dish = split(allrelevant[i], ',');
        int price = stoi(dish[2]);
        string name = dish[0];
        DishType type;
        if (dish[1] == "VEG") {
            type = VEG;
        } else if (dish[1] == "BVG") {
            type = BVG;
        } else if (dish[1] == "ALC") {
            type = ALC;
        } else if (dish[1] == "SPC") {
            type = SPC;
        }
        Dish dishToadd(id, name, price, type);
        menu.push_back(dishToadd);
        id++;
    }
}

//copy constructor
Restaurant::Restaurant(const Restaurant &oth): open{false}, tables(std::vector<Table*>()), menu(std::vector<Dish>()), actionsLog(std::vector<BaseAction*>()), customer_id(0), numoftables(0) {
    copy(oth);
}

//move constructor
Restaurant::Restaurant(Restaurant &&oth) : open{false}, tables(std::vector<Table*>()), menu(std::vector<Dish>()), actionsLog(std::vector<BaseAction*>()), customer_id(0), numoftables(0)  {
    steal(oth);
}
//destructor
Restaurant::~Restaurant() {
    clean();
}
//copy assingment operator
Restaurant &Restaurant::operator=(const Restaurant &oth) {
    clean();
    copy(oth);
    return *this;
}
//move assignment operator
Restaurant &Restaurant::operator=(Restaurant &&oth) {
    clean();
    steal(oth);
    return *this;
}
//utility function for move assignment operator which 'steals' all the data fron oth and inserts it to this.
void Restaurant::steal(Restaurant &oth) {
    this->open=oth.open;
    this->customer_id=oth.customer_id;

    this->tables=move(oth.tables);
    this->menu=move(oth.menu);
    this->actionsLog=move(oth.actionsLog);

    oth.customer_id=0;
    oth.open = false;

}
//utility function which helps us earase the data of a given restaurant
void Restaurant::clean() {
    this->open=false;
    this->customer_id=0;

    for (Table* obj : tables)
        delete obj;
    for (BaseAction* obj : actionsLog)
        delete obj;

    this->menu.clear();
    tables.clear();
    actionsLog.clear();
}
//utility function which creates a copy of all the data of a given restaurant
void Restaurant::copy(const Restaurant &oth) {

    open=oth.open;
    customer_id=oth.customer_id;

    for(Table* table : oth.tables){
        this->tables.push_back(new Table(*table));
    }

    for(BaseAction* action : oth.actionsLog){
        this->actionsLog.push_back(action->clone());
    }
    for(Dish dish : oth.menu){
        this->menu.push_back(Dish(dish));
    }
}
int Restaurant::getNumOfTables() const {
    return (int)tables.size();
}

Table* Restaurant::getTable(int ind) {
    return tables[ind];
}
Table* Restaurant::getTableById(int id) {
    for (unsigned int i = 0; i <tables.size() ; ++i) {
        if(tables.at(i)->getId()==id)
            return tables.at(i);
    }
    return nullptr;
}

const std::vector<BaseAction *> &Restaurant::getActionsLog() const {
    return actionsLog;
}

std::vector<Dish> &Restaurant::getMenu() {
    return menu;
}

void Restaurant::start() {
    open=true;
    cout<<"Restaurant is now open!"<<endl;
    string command="";
    while (command!="closeall"){
        string inputLine;
        getline(cin, inputLine);
        inputLine.erase(std::remove(inputLine.begin(), inputLine.end(), '\r'), inputLine.end());
        vector<string> input=split(inputLine,' ');
        command=input.at(0);
        BaseAction* action= nullptr;
        if(command=="open"){
            int tableId=stoi(input.at(1));
            vector<Customer*> customers;
            for (unsigned long i = 2; i < input.size(); ++i) {
                vector<string> c_info=split(input.at(i),',');
                Customer *customer = nullptr;
                if(c_info.at(1)=="veg")
                    customer=new VegetarianCustomer(c_info.at(0),customer_id);
                else if(c_info.at(1)=="chp")
                    customer=new CheapCustomer(c_info.at(0),customer_id);
                else if(c_info.at(1)=="alc")
                    customer=new AlchoholicCustomer(c_info.at(0),customer_id);
                else if(c_info.at(1)=="spc")
                    customer=new SpicyCustomer(c_info.at(0),customer_id);
                customer_id++;
                customers.push_back(customer);
            }
            action=new OpenTable(tableId,customers);
        } else if(command=="order"){
            int tableId=stoi(input.at(1));
            action=new Order(tableId);
        }else if(command=="move"){
            int src=stoi(input.at(1)),
                dst=stoi(input.at(2)),
                cid=stoi(input.at(3));
            action=new MoveCustomer(src,dst,cid);
        }else if(command=="close"){
            int tableId=stoi(input.at(1));
            action=new Close(tableId);
        }else if(command=="closeall"){
            action=new CloseAll();
        }else if(command=="log"){
            action=new PrintActionsLog();
        }else if(command=="menu"){
            action=new PrintMenu();
        }else if(command=="status"){
            int tableId=stoi(input.at(1));
            action=new PrintTableStatus(tableId);
        }else if(command=="backup"){
            action=new BackupRestaurant();
        }else if(command=="restore"){
            action=new RestoreResturant();
        }
        if(action) {
            action->act(*this);
            log(action);
            delete action;

        }
    }

}
//utility function that inserts a copy of a given baseAction to the actionsLog
void Restaurant::log(const BaseAction *action) {
    actionsLog.push_back(action->clone());
}
//utility function that splits a given string input according to a char declared by the user.
vector<string> Restaurant::split(const std::string &s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}



