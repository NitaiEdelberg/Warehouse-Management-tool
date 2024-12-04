#include "../include/WareHouse.h"
#include "../include/Parse.h"
#include "../include/Action.h"
#include "../include/Volunteer.h"
#include "../include/Customer.h"
#include "../include/Order.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

WareHouse::WareHouse(const string &configFilePath) : isOpen(false), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers(), customerCounter(0), volunteerCounter(0), orderCounter(0)
{
    Parse parse(configFilePath, *this);
    customers = parse.getCustomersList();
    volunteers = parse.getVolunteersList();
}
// rule of 5 implement:
// copy constructor:
WareHouse::WareHouse(WareHouse &other) : isOpen(other.isOpen), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers(), customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter), orderCounter(other.orderCounter)
{
    for (BaseAction *action : other.actionsLog)
    {
        actionsLog.push_back(action->clone());
    }
    for (Volunteer *volunteer : other.volunteers)
    {
        volunteers.push_back(volunteer->clone());
    }
    for (Order *order : other.pendingOrders)
    {   
        pendingOrders.push_back(order->clone());
    }
    for (Order *order : other.inProcessOrders)
    {
        inProcessOrders.push_back(order->clone());
    }
    for (Order *order : other.completedOrders)
    {
        completedOrders.push_back(order->clone());
    }
    for (Customer *customer : other.customers)
    {
        customers.push_back(customer->clone());
    }
}
void WareHouse::clear()
{ //delete and then clear
    while(!actionsLog.empty()) {
        delete actionsLog.back();
        actionsLog.pop_back();
    }
    while(!volunteers.empty()) {
        delete volunteers.back();
        volunteers.pop_back();
    }
    while(!pendingOrders.empty()) {
        delete pendingOrders.back();
        pendingOrders.pop_back();
    }
    while(!inProcessOrders.empty()) {
        delete inProcessOrders.back();
        inProcessOrders.pop_back();
    }
     while(!completedOrders.empty()) {
        delete completedOrders.back();
        completedOrders.pop_back();
    }
    while(!customers.empty()) {
        delete customers.back();
        customers.pop_back();
    }

}
// Destructor:
WareHouse::~WareHouse()
{
    clear();
}

// Copy Assignment Operator:
WareHouse &WareHouse ::operator=(WareHouse &other)
{
    if (&other != this)
    {
        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        customerCounter = other.customerCounter;
        clear();
        for (BaseAction *action : other.actionsLog)
        {
            actionsLog.push_back(action->clone());
        }
        for (Volunteer *volunteer : other.volunteers)
        {
            volunteers.push_back(volunteer->clone());
        }
        for (Customer *customer : other.customers)
        {
            customers.push_back(customer->clone());
        }
        for (Order *order : other.pendingOrders)
        {   
            pendingOrders.push_back(order->clone());
        }
        for (Order *order : other.inProcessOrders)
        {
            inProcessOrders.push_back(order->clone());
        }
        for (Order *order : other.completedOrders)
        {
            completedOrders.push_back(order->clone());
        }
    }
    return *this;
}
// move constructor
WareHouse::WareHouse(WareHouse &&other) : isOpen(other.isOpen), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers(), customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter), orderCounter(other.orderCounter)
    {
    for (BaseAction *action : other.actionsLog)
    {
        actionsLog.push_back(action);
    }
    for (Volunteer *volunteer : other.volunteers)
    {
        volunteers.push_back(volunteer);
    }
    for (Order *order : other.pendingOrders)
    {
        pendingOrders.push_back(order);
    }
    for (Order *order : other.inProcessOrders)
    {
        inProcessOrders.push_back(order);
    }
    for (Order *order : other.completedOrders)
    {
        completedOrders.push_back(order);
    }
    for (Customer *customer : other.customers)
    {
        customers.push_back(customer);
    }
    other.actionsLog.clear();
    other.volunteers.clear();
    other.pendingOrders.clear();
    other.inProcessOrders.clear();
    other.completedOrders.clear();
    other.customers.clear();
}
//move Assignment operator
WareHouse &WareHouse::operator=(WareHouse &&other)
{
    if(this != &other) 
    {
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;
        isOpen = other.isOpen;
        clear();
        for (BaseAction *action : other.actionsLog)
        {
            actionsLog.push_back(action);
        }
        for (Volunteer *volunteer : other.volunteers)
        {
            volunteers.push_back(volunteer);
        }
        for (Customer *customer : other.customers)
        {
            customers.push_back(customer);
        }
        for (Order *order : other.pendingOrders)
        {   
            pendingOrders.push_back(order);
        }
        for (Order *order : other.inProcessOrders)
        {
            inProcessOrders.push_back(order);
        }
        for (Order *order : other.completedOrders)
        {
            completedOrders.push_back(order);
        }
        other.actionsLog.clear();
        other.volunteers.clear();
        other.pendingOrders.clear();
        other.inProcessOrders.clear();
        other.completedOrders.clear();
        other.customers.clear();
    }
    return *this;
}

vector<string> splitWords(const string& str)
{
    vector<string> output;
    string word;
    for (unsigned int i = 0; i < str.size(); i++)
    {
        if (str[i] == ' ')
        {
            output.push_back(word);
            word = "";
        } else
        {
            word += str[i];
        }
    }
    output.push_back(word);
    return output;
}
void WareHouse::start()
{
    open();
    std::cout << "Warehouse is open!" << std::endl;
    while (isOpen)
    {
        std::cout << "\n" << "Enter action:" << std::endl;
        std::string action;
        std::getline(std::cin, action);
        std::vector<std::string> words = splitWords(action);

        if(words[0] == "step")
        {
            int numOfSteps = std::stoi(words[1]);
            SimulateStep* step = new SimulateStep(numOfSteps);
            step->act(*this);
        } else if (words[0] == "order")
        {
            int customerId = std::stoi(words[1]);
            AddOrder* order = new AddOrder(customerId);
            order->act(*this);
        }
        else if (words[0] == "customer")
        {
            string name = words[1];
            string type = words[2];
            int distance = std::stoi(words[3]);
            int maxOrder = std::stoi(words[4]);
            AddCustomer* customer = new AddCustomer(name, type, distance, maxOrder);
            customer->act(*this);
        }
        else if (words[0] == "orderStatus")
        {
            int orderId = std::stoi(words[1]);
            PrintOrderStatus* printStatus = new PrintOrderStatus(orderId);
            printStatus->act(*this);
        }
        else if (words[0] == "customerStatus")
        {
            int customerId = std::stoi(words[1]);
            PrintCustomerStatus* printStatus = new PrintCustomerStatus(customerId);
            printStatus->act(*this);
        }
        else if (words[0] == "volunteerStatus")
        {
            int volunteerId = std::stoi(words[1]);
            PrintVolunteerStatus* printStatus = new PrintVolunteerStatus(volunteerId);
            printStatus->act(*this);
        }
        else if (words[0] == "log")
        {
            PrintActionsLog* printLog = new PrintActionsLog();
            printLog->act(*this);
        }
        else if (words[0] == "close")
        {
            Close* close = new Close();
            close->act(*this);
        }
        else if (words[0] == "backup")
        {
            BackupWareHouse* backUp = new BackupWareHouse();
            backUp->act(*this);
        }
        else if (words[0] == "restore")
        {
            RestoreWareHouse* restore = new RestoreWareHouse();
            restore->act(*this);
        } else
        {
            std::cout << "Action doesn't exist";
        }
        
        words.clear();
    }
    

}
const vector<BaseAction *> &WareHouse::getActionsLog() const
{
    return actionsLog;
}
void WareHouse::addOrder(Order *order)
{
    pendingOrders.push_back(order);
}
void WareHouse::addCustomer(Customer *customer)
{
    customers.push_back(customer);
}
void WareHouse::addAction(BaseAction *action)
{
    actionsLog.push_back(action);
}
void WareHouse::printActionsLogs()
{
    string output;
    for (BaseAction *action : actionsLog)
    {
        output += action->toString();
        output += "\n";
    }
    std::cout << output << std::endl;
}
Customer &WareHouse::getCustomer(int customerId) const
{
    for (const auto &customer : customers)
    {
        if (customer->getId() == customerId)
        {
            return *customer;
        }
    }
    throw std::runtime_error("Customer doesn't exist");
}
Volunteer &WareHouse::getVolunteer(int volunteerId) const
{
    for (const auto &volunteer : volunteers)
    {
        if (volunteer->getId() == volunteerId)
        {
            return *volunteer;
        }
    }
    throw std::runtime_error("Volunteer doesn't exist");
}
Order &WareHouse::getOrder(int orderId) const
{
    for (const auto &order : pendingOrders)
    {
        if (order->getId() == orderId)
        {
            return *order;
        }
    }
    for (const auto &order : inProcessOrders)
    {
        if (order->getId() == orderId)
        {
            return *order;
        }
    }
    for (const auto &order : completedOrders)
    {
        if (order->getId() == orderId)
        {
            return *order;
        }
    }
    throw std::runtime_error("Order doesn't exist");
}
void WareHouse::close()
{   
    //print all orders
    string output;
    for (Order *order : pendingOrders)
    {
        output += "OrederID: " + std::to_string(order->getId()) + " , CustomerID: " + std::to_string(order->getCustomerId()) + " , OrderStatus: " + order->printOrderStatus();
        output += "\n";
    }
    for (Order *order : inProcessOrders)
    {
        output += "OrederID: " + std::to_string(order->getId()) + " , CustomerID: " + std::to_string(order->getCustomerId()) + " , OrderStatus: " + order->printOrderStatus();
        output += "\n";
    }
    for (Order *order : completedOrders)
    {
        output += "OrederID: " + std::to_string(order->getId()) + " , CustomerID: " + std::to_string(order->getCustomerId()) + " , OrderStatus: " + order->printOrderStatus();
        output += "\n";
    }
    std::cout << output << std::endl;

    //close the warehouse
    isOpen = false;
    clear();
}
void WareHouse::open()
{
    isOpen = true;
}
int WareHouse::setCustomerId()
{
    int output = customerCounter;
    ++customerCounter;
    return output;
}
int WareHouse::setVolunteerId()
{
    int output = volunteerCounter;
    ++volunteerCounter;
    return output;
}
int WareHouse::setOrderId()
{
    int output(orderCounter);
    ++orderCounter;
    return output;
}
void WareHouse::simulateOneStep()
{
    // assign orders
vector<Order *>::iterator itPending = pendingOrders.begin();

while (itPending != pendingOrders.end())
{
    bool assigned = false;

    for (Volunteer *volunteer : volunteers)
    {
        if (!assigned && volunteer->canTakeOrder(**itPending))
        {
            volunteer->acceptOrder(**itPending);
            if ((*itPending)->getStatus() == OrderStatus::PENDING)
            {
                (*itPending)->setStatus(OrderStatus::COLLECTING);
                (*itPending)->setCollectorId(volunteer->getId());
            }
            else
            {
                (*itPending)->setStatus(OrderStatus::DELIVERING);
                (*itPending)->setDriverId(volunteer->getId());
            }
            inProcessOrders.push_back(*itPending);
            itPending = pendingOrders.erase(itPending);
            assigned = true;
        }
    }
    
    if (!assigned)
    {
        ++itPending;
    }
}

    // preform a step
vector<Volunteer *>::iterator itVolunteers = volunteers.begin();

while (itVolunteers != volunteers.end())
{
    bool wasDeleted = false;
    Volunteer *volunteer = *itVolunteers;

    if (volunteer->isBusy())
    {
        volunteer->step();
        if (!volunteer->isBusy())
        {
            int orderId = volunteer->getCompletedOrderId();
            vector<Order *>::iterator itInProcess = inProcessOrders.begin();
            bool found = false;

            while (itInProcess != inProcessOrders.end())
            {
                Order *order = *itInProcess;

                if (!found && order->getId() == orderId)
                {
                    if (order->getStatus() == OrderStatus::COLLECTING)
                    {
                        pendingOrders.push_back(order);
                    }
                    else
                    {
                        order->setStatus(OrderStatus::COMPLETED);
                        completedOrders.push_back(order);
                    }

                    itInProcess = inProcessOrders.erase(itInProcess);
                    found = true;
                }
                else
                {
                    ++itInProcess;
                }
            }

            if (!volunteer->hasOrdersLeft())
            {
                delete volunteer;
                itVolunteers = volunteers.erase(itVolunteers);
                wasDeleted = true;
            }
        }
    }

    if (!wasDeleted)
    {
        ++itVolunteers;
    }
}

}