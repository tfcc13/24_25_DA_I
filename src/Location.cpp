//
// Created by tiago on 26/02/2025.
//

#include "Location.h"

#include <string>

class Location {

private:
    std::string name;
    int id;
    std::string code;
    bool canPark;
public:
    Location();

    Location(std::string name, std::string code, bool canPark);

    std::string getName();

    std::string getCode();

    bool getCanPark();

    void setName(std::string name);
    
    void setCode(std::string code);

    void setCanPark(bool canPark);
};
