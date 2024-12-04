#include "../include/Customer.h"
#include "../include/WareHouse.h"
#include "../include/Order.h"

//constructor
Customer::Customer(int id, const string &name, int locationDistance, int maxOrders) : id(id), name(name), locationDistance(locationDistance), maxOrders(maxOrders), ordersId() {}
// copy consturctor
Customer::Customer(const Customer &other) : id(other.id), name(other.name),locationDistance(other.locationDistance), maxOrders(other.maxOrders), ordersId(other.ordersId) {}
// destructor
Customer:: ~Customer() = default;
//getters 
const string &Customer:: getName() const
{
    return name;
}
int Customer::getId() const 
{
    return id;
}
int Customer::getCustomerDistance() const 
{
    return locationDistance;
}
int Customer::getMaxOrders() const 
{
    return maxOrders;
}
int Customer::getNumOrders() const 
{
    return ordersId.size();
}
const vector<int> &Customer::getOrdersIds() const 
{
    return ordersId;
}
//info
bool Customer::canMakeOrder() const {
    return static_cast<int>(ordersId.size()) < maxOrders;
}

//actions
int Customer::addOrder(int orderId) {
    if (canMakeOrder()) {
        ordersId.push_back(orderId);
        return orderId;
    }
    return -1;
}


//soldier Customer
//constructor
SoldierCustomer::SoldierCustomer(int id, const string &name, int locationDistance, int maxOrders) : Customer(id, name, locationDistance, maxOrders) {}
//clone
SoldierCustomer *SoldierCustomer::clone() const {
    return new SoldierCustomer(*this);
}

//Civilian Customer
//contructor
CivilianCustomer::CivilianCustomer(int id, const string &name, int locationDistance, int maxOrders) : Customer(id, name, locationDistance, maxOrders) {}
CivilianCustomer *CivilianCustomer::clone() const {
    return new CivilianCustomer(*this);
}



