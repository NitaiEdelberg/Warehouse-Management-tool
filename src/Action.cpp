#include "../include/Action.h"
#include "../include/WareHouse.h"
#include "../include/Volunteer.h"
#include <iostream>

extern WareHouse* backup;

BaseAction::BaseAction(): errorMsg(""), status(ActionStatus::ERROR){}
ActionStatus BaseAction::getStatus() const
{
    return status;
}
void BaseAction::complete()
{
    status = ActionStatus::COMPLETED;
}
void BaseAction::error(string errorMsg)
{
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
}
string BaseAction::getErrorMsg() const
{
    return errorMsg;
}
BaseAction:: ~BaseAction() = default;


SimulateStep::SimulateStep(int numOfSteps) : BaseAction(), numOfSteps(numOfSteps){}

void SimulateStep::act(WareHouse &wareHouse)
{
    for (int i = 0; i < numOfSteps; i++)
    {
        wareHouse.simulateOneStep(); // make sure simulatestep is done corectlly
    }
    
    complete();
    wareHouse.addAction(this);
}
string SimulateStep::toString() const
{
    string output = "simulateStep " + std::to_string(numOfSteps);

    if (getStatus() == ActionStatus::COMPLETED)
    {
        return output += " COMPLETED";
    } else
    {
        return output += " Error";
    } 
} 
SimulateStep *SimulateStep::clone() const
{
    return new SimulateStep(*this);
}


AddOrder::AddOrder(int id) : customerId(id){}

void AddOrder::act(WareHouse &wareHouse)
{
    int distance;
    bool maxorders = false;
    try
    {
        if(wareHouse.getCustomer(customerId).canMakeOrder())
        {
            distance = wareHouse.getCustomer(customerId).getCustomerDistance();
            int orderId(wareHouse.setOrderId());
            Order *newOrder = new Order(orderId,customerId,distance);
            wareHouse.getCustomer(customerId).addOrder(orderId);
            wareHouse.addOrder(newOrder);

            complete();
            wareHouse.addAction(this);
        } else
        {
            maxorders = true;
        }
        
    }
    catch(const std::runtime_error& e)
    {
        error("Cannot place this order");
        wareHouse.addAction(this);
        std::cerr << "Error: " << getErrorMsg() << '\n';
    }
    if (maxorders)
    {
        error("Cannot place this order");
        wareHouse.addAction(this);
        std::cout << "Error: " << getErrorMsg() << std::endl;
        // "Customer " << customerId << " reached his maxOrders limit"
    }
    
}

string AddOrder::toString() const
{
    string output = "order ";
    output += std::to_string(customerId);

    if (getStatus() == ActionStatus::COMPLETED)
    {
        return (output += " COMPLETED");
    } else
    {
        return (output += " ERROR");
    }   
} 
AddOrder *AddOrder::clone() const
{
    return new AddOrder(*this);
}



AddCustomer::AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders) : customerName(customerName), customerType(convert(customerType)), distance(distance), maxOrders(maxOrders){}

const CustomerType AddCustomer::convert(string customerType)
{
    if (customerType == "soldier")
    {
        return CustomerType::Soldier;
    } else {
        return CustomerType::Civilian;
    }
}
void AddCustomer::act(WareHouse &wareHouse) 
{
    int customerId = wareHouse.setCustomerId();
    if (customerType == CustomerType::Soldier)
    {
        SoldierCustomer *newSoliderCustomer = new SoldierCustomer(customerId, customerName, distance, maxOrders);
        wareHouse.addCustomer(newSoliderCustomer);
    } else {
        CivilianCustomer *newCvilianCustomer = new CivilianCustomer(customerId, customerName, distance, maxOrders);
        wareHouse.addCustomer(newCvilianCustomer);
    }
    complete();
    wareHouse.addAction(this);
}
AddCustomer *AddCustomer::clone() const
{
    return new AddCustomer(*this);
}
string AddCustomer::toString() const
{
    string output = "customer " + customerName;

    if (customerType == CustomerType::Civilian)
    {
        output += " civilian ";
    } else
    {
        output += " soldier ";
    }
    output += std::to_string(distance);
    output += " ";
    output += std::to_string(maxOrders);
    if (getStatus() == ActionStatus::COMPLETED)
    {
        return output += " COMPLETED";
    } else
    {
        return output += " ERROR";
    }
    return output;
}



PrintOrderStatus::PrintOrderStatus(int id) : orderId(id){}

void PrintOrderStatus::act(WareHouse &wareHouse)
{
    string output;

    try
    {
        output = wareHouse.getOrder(orderId).toString();

        std::cout << output << std::endl;

        complete();
        wareHouse.addAction(this);
    }
    catch(const std::runtime_error& e)
    {
        error("Order doesn't exist");
        wareHouse.addAction(this);
        std::cerr << "Error: " << e.what() << std::endl;;
    }
}
PrintOrderStatus *PrintOrderStatus::clone() const
{
    return new PrintOrderStatus(*this);
}
string PrintOrderStatus::toString() const
{
    string output = "orderStatus " + std::to_string(orderId);

    if (getStatus() == ActionStatus::COMPLETED)
    {
        return output += " COMPLETED";
    } else
    {
        return output += " ERROR";
    }
}


PrintCustomerStatus::PrintCustomerStatus(int id) : customerId(id){}

void PrintCustomerStatus::act(WareHouse &wareHouse)
{
    vector<int> orderIds;

    try
    {
        orderIds = wareHouse.getCustomer(customerId).getOrdersIds();
        string output = "CustomerID: " + std::to_string(customerId);
        output += "\n";

        for (int orderId : orderIds)
        {
            output += "OrderID: " + std::to_string(orderId);
            output += "\n";
            output += wareHouse.getOrder(orderId).printOrderStatus();
            output += "\n";
        }
        int numOrdersLeft = wareHouse.getCustomer(customerId).getMaxOrders() - wareHouse.getCustomer(customerId).getNumOrders();
        output += "NumOrdersLeft: " + std::to_string(numOrdersLeft);
        output += "\n";

        std::cout << output << std::endl;
        
        complete();
        wareHouse.addAction(this);
    }
    catch(const std::runtime_error& e)
    {
        error("Customer doesn't exist");
        wareHouse.addAction(this);
        std::cerr << "Error: " << e.what() << std::endl;
    }

    
}
PrintCustomerStatus *PrintCustomerStatus::clone() const
{
    return new PrintCustomerStatus(*this);
}
string PrintCustomerStatus::toString() const
{
    string output = "customerStatus " + std::to_string(customerId);

    if (getStatus() == ActionStatus::COMPLETED)
    {
        return output += " COMPLETED";
    } else
    {
        return output += " ERROR";
    }
}


PrintVolunteerStatus::PrintVolunteerStatus(int id) : volunteerId(id){}

void PrintVolunteerStatus::act(WareHouse &wareHouse)
{
    string output;
    try
    {
        output = wareHouse.getVolunteer(volunteerId).toString();
        
        std::cout << output << std::endl;   
        
        complete();
        wareHouse.addAction(this);
    }
    catch(const std::runtime_error& e)
    {
        error("Volunteer doesn't exist");
        wareHouse.addAction(this);
        std::cerr << "Error: " << e.what() << std::endl;
    }
 
}
PrintVolunteerStatus *PrintVolunteerStatus::clone() const
{
    return new PrintVolunteerStatus(*this);
}
string PrintVolunteerStatus::toString() const
{
    string output = "volunteerStatus " + std::to_string(volunteerId);

    if (getStatus() == ActionStatus::COMPLETED)
    {
        return output += " COMPLETED";
    } else
    {
        return output += " ERROR";
    }
}


PrintActionsLog::PrintActionsLog(){}

void PrintActionsLog::act(WareHouse &wareHouse)
{
    wareHouse.printActionsLogs();
    complete();
    wareHouse.addAction(this);
}
PrintActionsLog *PrintActionsLog::clone() const
{
    return new PrintActionsLog(*this);
}
string PrintActionsLog::toString() const
{
    string output = "log ";

    if (getStatus() == ActionStatus::COMPLETED)
    {
        return output + " COMPLETED";
    } else
    {
        return output + " ERROR";
    }
}


Close::Close(){};
void Close::act(WareHouse &wareHouse)
{
    complete();
    wareHouse.addAction(this);
    wareHouse.close();
    // make sure warehouse.close() do all the printing, free the memory and exit!!!
}
Close *Close::clone() const
{
    return new Close(*this);
}
string Close::toString() const
{
        string output = "close";

    if (getStatus() == ActionStatus::COMPLETED)
    {
        return output + " COMPLETED";
    } else
    {
        return output + " ERROR";
    }
}


BackupWareHouse::BackupWareHouse(){};

void BackupWareHouse::act(WareHouse &wareHouse)
{
    if(backup != nullptr)
    {
        *backup = wareHouse;
    } else
    {
        backup = new WareHouse(wareHouse);
    }
    
    complete();
    wareHouse.addAction(this);
}
BackupWareHouse *BackupWareHouse::clone() const
{
    return new BackupWareHouse(*this);
}
string BackupWareHouse::toString() const
{
    string output = "backup";

    if (getStatus() == ActionStatus::COMPLETED)
    {
        return output + " COMPLETED";
    } else
    {
        return output + " ERROR";
    }
}


RestoreWareHouse::RestoreWareHouse(){}

void RestoreWareHouse::act(WareHouse &wareHouse)
{
    try
    {
        if(backup == nullptr)
        {
            throw std::runtime_error("No backup available");
        } else
        {
            wareHouse = *backup; // make sure operator "=" copies everything correctly
            complete();
            wareHouse.addAction(this);
        }   
    }
    catch(const std::runtime_error& e)
    {
        error("No backup available");
        wareHouse.addAction(this);
        std::cerr << "Error: " << e.what() << '\n';
    }
}
RestoreWareHouse *RestoreWareHouse::clone() const
{
    return new RestoreWareHouse(*this);
}
string RestoreWareHouse::toString() const
{
    string output = "restore";

    if (getStatus() == ActionStatus::COMPLETED)
    {
        return output + " COMPLETED";
    } else
    {
        return output + " ERROR";
    }
}