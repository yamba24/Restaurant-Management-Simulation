

#include "../include/Action.h"
#include "../include/Restaurant.h"
extern Restaurant* backup;
BaseAction::BaseAction()
    :errorMsg("")
    ,status(PENDING){

}

ActionStatus BaseAction::getStatus() const {
    return status;
}

BaseAction::~BaseAction() = default;

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

void BaseAction::complete() {
    status=COMPLETED;
}

void BaseAction::error(std::string errorMsg) {
    cout<<"Error: "<<errorMsg<<endl;
    this->errorMsg=errorMsg;
    status=ERROR;
}

void BaseAction::copyStatus(BaseAction &action) const{
    action.errorMsg=this->getErrorMsg();
    action.status=this->status;
}


OpenTable::OpenTable(int id, vector<Customer *> &customersList)
:BaseAction()
,tableId(id)
,customers(move(customersList)){}


void OpenTable::act(Restaurant &restaurant) {
    if (restaurant.getTableById(tableId) == nullptr || restaurant.getTableById(tableId)->isOpen()) {
        error("Table does not exist or is already open");
        return;
    }
    restaurant.getTableById(tableId)->openTable();
    for (unsigned int i = 0; i < customers.size() ; ++i) {
        restaurant.getTableById(tableId)->addCustomer(customers.at(i));
    }
    complete();
}

std::string OpenTable::toString() const {
    string str = "open "+to_string(tableId);
    for (unsigned int i = 0; i < customers.size() ; ++i) {
        str= str+" "+customers.at(i)->toString();
    }

    string s=" Completed";
    if(!getErrorMsg().empty())
        s=" Error: "+getErrorMsg();

    return str+s;
}
BaseAction *OpenTable::clone() const{
    auto* copy = new OpenTable(*this);
    copyStatus(*copy);
    return copy;
}
//constructor
OpenTable::OpenTable(const OpenTable &oth)
        :BaseAction(oth)
        ,tableId(oth.tableId)
        ,customers(oth.deepCopyOfCustumers()){
}
//copy constructor
OpenTable::OpenTable(OpenTable &&oth)
        :BaseAction(oth)
        ,tableId(oth.tableId)
        ,customers(oth.deepCopyOfCustumers()){
    oth.~OpenTable();
}
//destructor
OpenTable::~OpenTable(){
    for(Customer* customer : customers) {
        delete customer;
        customer= nullptr;
    }
    customers.clear();
}
//utility function that makes a deep copy of the vector of customers
std::vector<Customer *> OpenTable::deepCopyOfCustumers() const {
    vector<Customer*> output;
    for(Customer* customer : customers)
        output.push_back(customer->clone());
    return output;
}

Order::Order(int id)
:BaseAction(),tableId(id){}

void Order::act(Restaurant &restaurant) {
    Table* table=restaurant.getTableById(tableId);
    if(restaurant.getTableById(tableId) == nullptr || !restaurant.getTableById(tableId)->isOpen()) {
        error("Table does not exist or is not open");
        return;
    }
    vector<Dish> menu=restaurant.getMenu();
    unsigned long prevSize=restaurant.getTableById(tableId)->getOrders().size();
    restaurant.getTableById(tableId)->order(menu);
    vector<OrderPair> orders=table->getOrders();
    unsigned long newSize=restaurant.getTableById(tableId)->getOrders().size();

    for (unsigned int i = prevSize; i < newSize; ++i) {
        cout<<table->getCusNameById(orders.at(i).first)<<" ordered "<<orders.at(i).second.getName()<<endl;
    }
    complete();
}

std::string Order::toString() const {
    string s=" Completed";
    if(!getErrorMsg().empty())
        s=" Error: "+getErrorMsg();
    return "order "+to_string(tableId)+ s;
}

BaseAction *Order::clone() const{
    auto* copy = new Order(this->tableId);
    copyStatus(*copy);

    return copy;
}

MoveCustomer::MoveCustomer(int src, int dst, int customerId)
:BaseAction()
,srcTable(src)
,dstTable(dst)
,id(customerId){

}

void MoveCustomer::act(Restaurant &restaurant) {
    if(!restaurant.getTableById(srcTable) || !restaurant.getTableById(dstTable) || !restaurant.getTableById(srcTable)->getCustomer(id)|| !restaurant.getTableById(dstTable)->isOpen() || restaurant.getTableById(dstTable)->getCapacity()<=(int)restaurant.getTableById(dstTable)->getCustomers().size()){
        error("Cannot move customer");
        return;
    }
    restaurant.getTableById(dstTable)->addCustomer(restaurant.getTableById(srcTable)->getCustomer(id));
    restaurant.getTableById(srcTable)->removeCustomer(id);

    restaurant.getTableById(dstTable)->insertOrder(restaurant.getTableById(srcTable)->extractOrder(id));

    if(restaurant.getTableById(srcTable)->getCustomers().empty())
        restaurant.getTableById(srcTable)->closeTable();
    complete();
}

std::string MoveCustomer::toString() const {
    string s=" Completed";
    if(!getErrorMsg().empty())
        s=" Error: "+getErrorMsg();
    return "move "+to_string(srcTable)+" "+to_string(dstTable)+" "+to_string(id)+s;
}

BaseAction *MoveCustomer::clone() const{
    auto* copy = new MoveCustomer(this->srcTable,this->dstTable,this->id);
    copyStatus(*copy);

    return copy;
}

Close::Close(int id):BaseAction(),tableId(id){}

void Close::act(Restaurant &restaurant) {
    if(!restaurant.getTableById(tableId) || !restaurant.getTableById(tableId)->isOpen()) {
        error("Table does not exist or is not open");
        return;
    }
    int bill=restaurant.getTableById(tableId)->getBill();
    restaurant.getTableById(tableId)->closeTable();
    cout<<"Table "<<tableId<<" was closed. Bill "<< bill <<"NIS"<<endl;
    complete();
}

std::string Close::toString() const {
    string s=" Completed";
    if(!getErrorMsg().empty())
        s=" Error: "+getErrorMsg();
    return "close "+to_string(tableId)+ s;
}

BaseAction *Close::clone() const{
    auto* copy = new Close(this->tableId);
    copyStatus(*copy);

    return copy;
}

CloseAll::CloseAll():BaseAction() {}

void CloseAll::act(Restaurant &restaurant) {
    for (int i = 0; i < restaurant.getNumOfTables(); ++i) {
        if(restaurant.getTableById(i)->isOpen()){
            Close* close=new Close(restaurant.getTableById(i)->getId());
            close->act(restaurant);
            delete close;
        }
    }
    complete();
}

std::string CloseAll::toString() const {
    string s=" Completed";
    if(!getErrorMsg().empty())
        s=" Error: "+getErrorMsg();
    return "closeall"+ s;
}

BaseAction *CloseAll::clone() const{
    auto* copy = new CloseAll();

    return copy;
}

PrintMenu::PrintMenu() {}

void PrintMenu::act(Restaurant &restaurant) {
    for (unsigned int i = 0; i < restaurant.getMenu().size() ; ++i) {
        Dish d=Dish(restaurant.getMenu().at(i));
        cout<<d.getName()<<" "<<Dish::type2string(d.getType())<<" "<<d.getPrice()<<"NIS"<<endl;
    }
    complete();
}

std::string PrintMenu::toString() const {
    string s=" Completed";
    if(!getErrorMsg().empty())
        s=" Error: "+getErrorMsg();
    return "menu"+ s;
}

BaseAction *PrintMenu::clone() const{
    auto* copy = new PrintMenu();
    copyStatus(*copy);

    return copy;
}

PrintTableStatus::PrintTableStatus(int id)
        :BaseAction()
        ,tableId(id){}

void PrintTableStatus::act(Restaurant &restaurant) {
    string s="open";
    if(!restaurant.getTableById(tableId)->isOpen())
        s="closed";
    cout<<"Table "<<to_string(tableId)<<" status: "<< s <<endl;
    if(!restaurant.getTableById(tableId)->isOpen()) {
        complete();
        return;
    }
    cout<<"Customers:"<<endl;
    for (unsigned int i = 0; i <restaurant.getTableById(tableId)->getCustomers().size() ; ++i) {
        cout<<restaurant.getTableById(tableId)->getCustomers().at(i)->getId()<<" "+restaurant.getTableById(tableId)->getCustomers().at(i)->getName()<<endl;
    }
    cout<<"Orders:"<<endl;
    for (unsigned int j = 0; j <restaurant.getTableById(tableId)->getOrders().size() ; ++j) {
        Dish d=Dish(restaurant.getTableById(tableId)->getOrders().at(j).second);
        cout<<d.getName()<<" "<<d.getPrice()<<"NIS"<<" "<<restaurant.getTableById(tableId)->getOrders().at(j).first<<endl;
    }
    cout<<"Current Bill: "<<restaurant.getTableById(tableId)->getBill()<<"NIS"<<endl;
    complete();
}

std::string PrintTableStatus::toString() const {
    string s = " Completed";
    if (!getErrorMsg().empty())
        s = " Error: " + getErrorMsg();
    return "status "+to_string(tableId) + s;

}

BaseAction *PrintTableStatus::clone() const{
    auto* copy =new PrintTableStatus(this->tableId);
    copyStatus(*copy);

    return copy;
}

PrintActionsLog::PrintActionsLog():BaseAction(){}

void PrintActionsLog::act(Restaurant &restaurant) {
    for (unsigned int i = 0; i < restaurant.getActionsLog().size(); ++i) {
        cout << restaurant.getActionsLog().at(i)->toString() << endl;
    }
    complete();
}

std::string PrintActionsLog::toString() const {
    string s=" Completed";
    if(!getErrorMsg().empty())
        s=" Error: "+getErrorMsg();
    return "log"+ s;
}

BaseAction *PrintActionsLog::clone() const{
    auto* copy =new PrintActionsLog();
    copyStatus(*copy);

    return copy;
}

BackupRestaurant::BackupRestaurant():BaseAction() {}

void BackupRestaurant::act(Restaurant &restaurant) {
    if(backup)
        delete backup;
    backup=new Restaurant(restaurant);
    complete();
}

std::string BackupRestaurant::toString() const {
    string s=" Completed";
    if(!getErrorMsg().empty())
        s=" Error: "+getErrorMsg();
    return "backup"+ s;
}

BaseAction *BackupRestaurant::clone() const{
    auto* copy =new BackupRestaurant();
    copyStatus(*copy);

    return copy;
}

RestoreResturant::RestoreResturant():BaseAction() {

}

void RestoreResturant::act(Restaurant &restaurant) {
    if(!backup){
        error("No backup available");
        return;
    }
    restaurant=*backup;
    complete();
}

std::string RestoreResturant::toString() const {
    string s=" Completed";
    if(!getErrorMsg().empty())
        s=" Error: "+getErrorMsg();
    return "restore"+ s;
}

BaseAction *RestoreResturant::clone() const{
    auto* copy=new RestoreResturant();
    copyStatus(*copy);

    return copy;
}
