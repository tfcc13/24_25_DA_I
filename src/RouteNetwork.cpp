//
// Created by tiago on 26/02/2025.
//

#include "RouteNetwork.h"

#include <fstream>
#include <sstream>

#include "Route.h"

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

bool RouteNetwork::parseRoute(const std::string& route_file) {
    std::ifstream routes(route_file);
    std::string line ;

    if (!std::getline(routes,line)) {
        return false;
    }

    std::string orig_location, dest_location, walking_time, driving_time;
    std::istringstream iss(line);
    char sep = ',';
    double w_time, d_time;
    while (std::getline(routes,line)) {
        iss.clear();
        iss.str(line);
        getline(iss,orig_location,sep);
        getline(iss,dest_location,sep);
        getline(iss,driving_time,sep);
        getline(iss,walking_time,sep);

        if (driving_time == "X") {
            d_time = INT_MAX;
        }
        else {
            d_time = std::stod(driving_time);
        }
        w_time = std::stod(walking_time);

        addEdge(orig_location,dest_location,w_time,d_time);
    }

    routes.close();

    return true;

}

bool RouteNetwork::parseData(const std::string &location_data, const std::string &route_data) {

    if (!parseLocation(location_data)) {
        std::cout << "Location file does not exist" << std::endl;
        return false;
    }

    if (!parseRoute(route_data)) {
        std::cout << "Route file does not exist" << std::endl;
        return false;
    }
    return true;
}



