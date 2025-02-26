//
// Created by tiago on 26/02/2025.
//

#include "RouteNetwork.h"

#include <fstream>
#include <sstream>

RouteNetwork::RouteNetwork() {
    route_network_ = nullptr;
    locations_ = nullptr;
}

RouteNetwork::~RouteNetwork() {
    for (auto& pair : locations_) {
        delete pair.second;
    }
    delete route_network_;
    delete locations_;

}

bool RouteNetwork::parseLocation(const std::string& location_file) {

    std::ifstream locations(location_file);
    std::string line ;

    if (!std::getline(locations,line)) {
        return false;
    }

    std::string location, id, code, park;
    std::istringstream iss(line);

    bool canPark;
    char sep = ',';

    while (std::getline(locations,line)) {
        iss.clear();
        iss.str(line);
        getline(iss,id,sep);
        getline(iss,code,sep);
        getline(iss,park,sep);

        canPark = (std::stoi(park) == 1);
        Location* tempLocation = new Location(location,id,code,canPark); ;
        this->addVertex(tempLocation);

        this->locations_->insert(std::make_pair(code,tempLocation));

    }

    locations.close();

    return true;



}


