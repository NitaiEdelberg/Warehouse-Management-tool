#include "../include/Order.h"



Order::Order(int id, int customerId, int distance) : id(id), customerId(customerId), distance(distance), status(OrderStatus::PENDING), collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER){}
Order::Order(const Order &other) : id(other.id), customerId(other.customerId), distance(other.distance), status(other.status), collectorId(other.collectorId), driverId(other.driverId){}
int Order::getId() const
{
    return id;
}
int Order::getCustomerId() const
{
    return customerId;
}
void Order::setStatus(OrderStatus status)
{
    this->status = status;//???
}
void Order::setCollectorId(int collectorId)
{
    this->collectorId = collectorId;
}
void Order::setDriverId(int driverId)
{
    this->driverId = driverId;
}
int Order::getCollectorId() const
{
    return collectorId;
}
int Order::getDriverId() const
{
    return driverId;
}
OrderStatus Order::getStatus() const
{
    return status;
}
const string Order::toString() const
{
    string output;
    output += "OrderId: " + std::to_string(id);
    output += "\n";

    output += printOrderStatus(); 
    output += "\n";

    output += "CustomerID: " + std::to_string(getCustomerId());
    output += "\n";

    output += "Collector: ";
    if (getCollectorId() != NO_VOLUNTEER)
    {
        output += std::to_string(getCollectorId());
    } else
    {
        output += "None";
    }
    output += "\n";
    
    output += "Driver: ";
    if (getDriverId() != NO_VOLUNTEER)
    {
        output += std::to_string(getDriverId());
    } else
    {
        output += "None";
    }
    output += "\n";
    return output;
}
const string Order::printOrderStatus() const
{
    string output = "OrderStatus: ";
    if (this->status == OrderStatus::PENDING)
    {
        output += "PENDING";
    } else if (this->status == OrderStatus::COLLECTING)
    {
        output += "COLLECTING";
    } else if (this->status == OrderStatus::DELIVERING)
    {
        output += "DELIVERING";
    } else
    {
        output += "COMPLETED";
    }

    return output;
}
int Order::getDistance() const
{
    return distance;
}
Order *Order::clone() const
{
    return new Order(*this);
}

