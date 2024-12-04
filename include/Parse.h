#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;
#include "WareHouse.h"

class WareHouse;

class Parse{

    public:
        Parse (const string &configFilePath, WareHouse &wareHouse);
        vector<Customer*> &getCustomersList();
        vector<Volunteer*> &getVolunteersList();

    private:
        vector<Customer*> customersList;
        vector<Volunteer*> volunteersList;
};