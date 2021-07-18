

#include "../include/Table.h"
#include <utility>

using namespace std;
int Table::amount = 0;

Table::Table(int t_capacity)
        : id(amount++), capacity(t_capacity), open(false), customersList(), orderList() {


}
//constructor
Table::Table(int t_id, int t_capacity, bool t_open)
        : id(t_id), capacity(t_capacity), open(t_open), customersList(), orderList() {}
//copy constructor
Table::Table(const Table &oth)
        : Table(oth.id, oth.capacity, oth.open) {
    //customersList=tbl.customersList;
    for (unsigned int i = 0; i < oth.customersList.size(); ++i) {
        customersList.push_back(oth.customersList[i]->clone());
    }

    for (unsigned int i = 0; i < oth.orderList.size(); ++i) {
        orderList.push_back(OrderPair(oth.orderList[i]));
    }

}
//move constructor
Table::Table(Table &&tbl)
        : Table(tbl.id, tbl.capacity, tbl.open) {
    tbl.customersList.clear();

    customersList = move(tbl.customersList);
    orderList = move(tbl.orderList);
}

//destructor
Table::~Table() {
    id = -1;
    capacity = -1;
    open = false;
    for (Customer *customer : customersList) {

        delete customer;
    }
    customersList.clear();
    orderList.clear();
}
//copy assignment operator
Table &Table::operator=(const Table &tbl) {
    id = tbl.id;
    open = tbl.open;
    capacity = tbl.capacity;
    for (unsigned int i = 0; i < customersList.size(); ++i) {
        customersList.push_back(customersList[i]->clone());
    }

    for (unsigned int i = 0; i < orderList.size(); ++i) {
        orderList.push_back(OrderPair(orderList[i]));
    }
    return *this;
}

//move assignment operator
Table &Table::operator=(Table &&tbl) {
    id = tbl.id;
    open = tbl.open;
    capacity = tbl.capacity;

    customersList = move(tbl.customersList);
    orderList = move(tbl.orderList);
    return *this;
}

int Table::getCapacity() const {
    return capacity;
}

void Table::openTable() {
    open = true;
}

void Table::closeTable() {

    for (Customer *customer : customersList)
        delete customer;
    customersList.clear();
    orderList.clear();
    open = false;
}

bool Table::isOpen() {
    return open;
}

Customer *Table::getCustomer(int id) {
    for (Customer *customer : customersList) {
        if (customer->getId() == id)
            return customer;
    }
    return nullptr;
}

void Table::addCustomer(Customer *customer) {
    customersList.push_back(customer->clone());
}

std::vector<Customer *> &Table::getCustomers() {
    return customersList;
}

std::vector<OrderPair> &Table::getOrders() {
    return orderList;
}

void Table::removeCustomer(int id) {
    int idToRemove = -1;
    for (unsigned int i = 0; i < customersList.size(); ++i)
        if (customersList.at(i)->getId() == id)
            idToRemove = i;
    delete customersList.at(idToRemove);
    customersList.erase(customersList.begin() + idToRemove);
}

void Table::order(const std::vector<Dish> &menu) {

    for (Customer *&customer: customersList) {
        vector<int> dishsIds = customer->order(menu);
        for (int dishId : dishsIds) {
            for (Dish dishInMenu : menu) {
                if (dishInMenu.getId() == dishId) {
                    orderList.push_back(OrderPair(customer->getId(), dishInMenu));
                }
            }
        }
    }
}

int Table::getId() {
    return id;
}

int Table::getBill() {
    int bill = 0;
    for (unsigned int i = 0; i <orderList.size() ; ++i) {
        bill= bill+orderList.at(i).second.getPrice();
    }
    return bill;
}

std::string Table::getCusNameById(int id) {
    for (unsigned int i = 0; i < customersList.size(); ++i) {
        if (customersList.at(i)->getId() == id)
            return customersList.at(i)->getName();
    }
    return "";
}
//utility function for the baseAction moveCustomer, which extract its orders from its source table.
    std::vector<OrderPair> Table::extractOrder(int customer_id) {
    vector<OrderPair> customer_order = vector<OrderPair>();
    vector<OrderPair> newOrderList = vector<OrderPair>();
    for (unsigned int i = 0; i < orderList.size() ; ++i) {
        if (orderList.at(i).first == customer_id)
            customer_order.push_back(orderList.at(i));
        else
            newOrderList.push_back(orderList.at(i));
    }
    orderList.clear();
    orderList = move(newOrderList);
    return customer_order;
}
//utility function for the baseAction moveCustomer, which inserts its orders to its destination table.
void Table::insertOrder(std::vector<OrderPair> orders) {
    for (unsigned int i = 0; i < orders.size(); ++i)
        orderList.push_back(orders.at(i));
    orders.clear();

}

