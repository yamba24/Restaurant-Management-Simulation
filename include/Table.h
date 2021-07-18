#ifndef TABLE_H_
#define TABLE_H_

#include "Customer.h"
#include "Dish.h"

typedef std::pair<int, Dish> OrderPair;

class Table{
public:
    Table(int t_capacity);
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const std::vector<Dish> &menu);
    void openTable();
    void closeTable();
    int getBill();
    bool isOpen();


    ~Table();               //destructor
    int getId();
    std::string getCusNameById(int id);
    Table(int t_id,int t_capacity, bool t_open);//Constractor
    Table(const Table& tbl);//copy constructor
    Table(Table &&tbl);     //move constructor
    Table& operator=(const Table& tbl);//copy assignment operator
    Table& operator=(Table&& tbl);     //move assignment operator
    std::vector<OrderPair> extractOrder(int customer_id);
    void insertOrder(std::vector<OrderPair> orders);
    static int amount;
private:
    int id;
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order in a table - (customer_id, Dish)
};


#endif
