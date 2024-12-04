#include "../include/Volunteer.h"
#include "../include/WareHouse.h"
#include "../include/Order.h"
#include <iostream>

//Volunteer constructor
Volunteer::Volunteer(int id, const string &name) : completedOrderId(NO_ORDER), activeOrderId(NO_ORDER), id(id), name(name) {}

//Volunteer destructor
Volunteer:: ~Volunteer() = default;
int Volunteer::getId() const
{
    return id;
}
const string &Volunteer::getName() const
{
    return name;
}
int Volunteer::getActiveOrderId() const 
{
    return activeOrderId; 
}
int Volunteer::getCompletedOrderId() const 
{
    return completedOrderId;
}
bool Volunteer::isBusy() const
{
    return activeOrderId != NO_ORDER;
}



CollectorVolunteer::CollectorVolunteer(int id, const string &name, int coolDown) : Volunteer(id, name), coolDown(coolDown), timeLeft(0) {}
CollectorVolunteer *CollectorVolunteer::clone() const
{
    return new CollectorVolunteer(*this);
}
void CollectorVolunteer::step()
{
    if (timeLeft != 0 && decreaseCoolDown()){
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
    }
}
int CollectorVolunteer::getCoolDown() const
{
    return coolDown;
}
int CollectorVolunteer::getTimeLeft() const
{
    return timeLeft;
}
bool CollectorVolunteer::decreaseCoolDown()
{
    --timeLeft;
    return timeLeft == 0;
}
bool CollectorVolunteer::hasOrdersLeft() const
{
    return true;
}
bool CollectorVolunteer::canTakeOrder(const Order &order) const
{
    return (!isBusy() && hasOrdersLeft() && order.getStatus() == OrderStatus::PENDING);
}
void CollectorVolunteer::acceptOrder(const Order &order)
{
    try {
        if(!canTakeOrder(order)){
            throw "Can't accept order!";
        } else {
            activeOrderId = order.getId();
            timeLeft = coolDown;
        }
    }
    catch(const char* exp){
        std::cout << "Error : " << exp <<" Order ID : " << order.getId() << std::endl;
    }
}
string CollectorVolunteer::toString() const
{
    string output;
    // print id
    output += "VoulunteerID: " + std::to_string(getId());
    output += "\n";

    // print everything else
    bool busy = isBusy();
    if (!busy)
    {
        output += "isBusy: False";
        output += "\n";
        output += "OrderID: None";
        output += "\n";
        output += "TimeLeft: None";
        output += "\n";
    } else
    {
        output += "isBusy: True";
        output += "\n";
        output += "OrderID: " + std::to_string(getActiveOrderId());
        output += "\n";
        output += "TimeLeft: " + std::to_string(getTimeLeft());
        output += "\n";  
    }

    // print Orders Left
    output += "OrdersLeft: No Limit";
    output += "\n";
        
    return output;
}

LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, const string &name, int coolDown, int maxOrders) : CollectorVolunteer(id, name, coolDown), maxOrders(maxOrders), ordersLeft(maxOrders) {}
LimitedCollectorVolunteer *LimitedCollectorVolunteer::clone() const
{
    return new LimitedCollectorVolunteer(*this);
}
bool LimitedCollectorVolunteer::hasOrdersLeft() const
{
    return ordersLeft > 0;
}
bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const
{
    return (!isBusy() && hasOrdersLeft() && order.getStatus() == OrderStatus::PENDING);
}
void LimitedCollectorVolunteer::acceptOrder(const Order &order)
{
    CollectorVolunteer::acceptOrder(order);
    --ordersLeft;
}
int LimitedCollectorVolunteer::getMaxOrders() const
{
    return maxOrders;
}
int LimitedCollectorVolunteer::getNumOrdersLeft() const
{
    return ordersLeft;
}
string LimitedCollectorVolunteer::toString() const
{
    string output;
    // print id
    output += "VoulunteerID: " + std::to_string(getId());
    output += "\n";

    // print everything else
    bool busy = isBusy();
    if (!busy)
    {
        output += "isBusy: False";
        output += "\n";
        output += "OrderID: None";
        output += "\n";
        output += "TimeLeft: None";
        output += "\n";
    } else
    {
        output += "isBusy: True";
        output += "\n";
        output += "OrderID: " + std::to_string(getActiveOrderId());
        output += "\n";
        output += "TimeLeft: " + std::to_string(getTimeLeft());
        output += "\n";  
    }

    // print orders left
        output += "OrdersLeft: " + std::to_string(ordersLeft);
        output += "\n";
        
    return output;
}




DriverVolunteer::DriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep) : Volunteer(id, name), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(0) {}
DriverVolunteer *DriverVolunteer::clone() const
{
    return new DriverVolunteer(*this);
}
int DriverVolunteer::getDistanceLeft() const
{
    return distanceLeft;
}
int DriverVolunteer::getMaxDistance() const
{
    return maxDistance;
}
int DriverVolunteer::getDistancePerStep() const
{
    return distancePerStep;
}
bool DriverVolunteer::decreaseDistanceLeft()
{
    distanceLeft = distanceLeft - distancePerStep;
    return distanceLeft <= 0;
}
bool DriverVolunteer::hasOrdersLeft() const
{
    return true;
}
bool DriverVolunteer::canTakeOrder(const Order &order) const
{
    return (!isBusy() && order.getDistance() <= maxDistance && hasOrdersLeft() && order.getStatus() == OrderStatus::COLLECTING);
}
void DriverVolunteer::acceptOrder(const Order &order)
{
    try {
        if(!canTakeOrder(order)){
            throw "Can't accept order!";
        } else {
            activeOrderId = order.getId();
            distanceLeft = order.getDistance();
        }
    }
    catch(const char* exp){
        std::cout << "Error : " << exp <<" Order ID : " << order.getId() << std::endl;
    }

}
void DriverVolunteer::step()
{
    if (distanceLeft > 0 && decreaseDistanceLeft()){
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
        distanceLeft = 0;
    }
}
string DriverVolunteer::toString() const
{
    string output;
    // print id
    output += "VoulunteerID: " + std::to_string(getId());
    output += "\n";

    // print everything else
    bool busy = isBusy();
    if (!busy)
    {
        output += "isBusy: False";
        output += "\n";
        output += "OrderID: None";
        output += "\n";
        output += "TimeLeft: None";
        output += "\n";
    } else
    {
        output += "isBusy: True";
        output += "\n";
        output += "OrderID: " + std::to_string(getActiveOrderId());
        output += "\n";
        output += "TimeLeft: " + std::to_string(getDistanceLeft());
        output += "\n";  
    }

    // print orders left
        output += "OrdersLeft: No Limit";
        output += "\n";
    return output;
}


LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep, int maxOrders) : DriverVolunteer(id, name, maxDistance, distancePerStep), maxOrders(maxOrders), ordersLeft(maxOrders) {}
LimitedDriverVolunteer *LimitedDriverVolunteer:: clone() const
{
     return new LimitedDriverVolunteer(*this);
}
int LimitedDriverVolunteer::getMaxOrders() const
{
    return maxOrders;
}
int LimitedDriverVolunteer::getNumOrdersLeft() const
{
    return ordersLeft;
}
bool LimitedDriverVolunteer::hasOrdersLeft() const
{
    return ordersLeft > 0;
}
bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const
{
    return (!isBusy() && order.getDistance() <= getMaxDistance() && hasOrdersLeft() && order.getStatus() == OrderStatus::COLLECTING);
}
void LimitedDriverVolunteer::acceptOrder(const Order &order)
{
    DriverVolunteer::acceptOrder(order);
    --ordersLeft;
}
string LimitedDriverVolunteer::toString() const
{
    string output;
    // print id
    output += "VoulunteerID: " + std::to_string(getId());
    output += "\n";

    // print everything else
    bool busy = isBusy();
    if (!busy)
    {
        output += "isBusy: False";
        output += "\n";
        output += "OrderID: None";
        output += "\n";
        output += "TimeLeft: None";
        output += "\n";
    } else
    {
        output += "isBusy: True";
        output += "\n";
        output += "OrderID: " + std::to_string(getActiveOrderId());
        output += "\n";
        output += "TimeLeft: " + std::to_string(getDistanceLeft());
        output += "\n";  
    }

    // print orders left
        output += "OrdersLeft: " + std::to_string(ordersLeft);
        output += "\n";

    return output;
}
