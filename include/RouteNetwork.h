//
// Created by tiago on 26/02/2025.
//

#ifndef ROUTENETWORK_H
#define ROUTENETWORK_H

#include <unordered_map>
#include "Graph.h"
#include "Location.h"

class Location;

class RouteNetwork : private Graph<std::string> {
private:
    RouteNetwork *route_network_;
    std::unordered_map<std::string ,Location*>* locations_;

    bool parseLocation(const std::string& location_file);
    bool parseRoute(const std::string& route_file);

public:
    RouteNetwork();
    ~RouteNetwork();
    bool parseData(const std::string& location_data, const std::string& route_data);
    std::unordered_map<std::string ,Location*>* getLocations();
    int getNumberOfLocations();

    Location* getLocationById(int id);
};

#endif //ROUTENETWORK_H
