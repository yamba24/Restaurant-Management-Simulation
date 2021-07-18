#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <vector>
#include <string>
#include "Table.h"
#include "Action.h"

using namespace std;

class Restaurant{		
public:
	Restaurant();

	explicit Restaurant(const std::string &configFilePath);
    void start();
    int getNumOfTables() const;
    Table* getTable(int ind);
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Dish>& getMenu();

    ~Restaurant();
	static std::vector<std::string> split(const std::string& s, char delimiter);
    Table* getTableById(int id);
    Restaurant(const Restaurant& other);
    Restaurant(Restaurant&& other) ;
    Restaurant& operator=(const Restaurant& oth);
    Restaurant& operator=(Restaurant&& oth) ;
    void steal(Restaurant& oth);
    void clean();
	void copy(const Restaurant& oth);
	void log(const BaseAction* action);

private:
    bool open{};
    std::vector<Table*> tables;
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;
    int customer_id;
    int numoftables;
};



#endif