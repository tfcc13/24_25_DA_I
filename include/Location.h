//
// Created by tiago on 26/02/2025.
//

#ifndef LOCATION_H
#define LOCATION_H

#include <Graph.h>
#include <string>

class Location : public Vertex<std::string>{
private:
    std::string name_;
    std::string id_;
    std::string code_;
    bool canPark_;
public:
    Location(std::string name, std::string id, std::string code, bool canPark);

    std::string getName();

    std::string getId();

    std::string getCode();

    bool getCanPark();

    void setName(std::string name);

    void setCode(std::string code);

    void setCanPark(bool canPark);
};


#endif //LOCATION_H
