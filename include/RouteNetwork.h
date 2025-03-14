//
// Created by tiago on 26/02/2025.
//

#ifndef ROUTENETWORK_H
#define ROUTENETWORK_H

#include <unordered_map>
#include <unordered_set>

#include "Graph.h"
#include "Location.h"

class Location;

class RouteNetwork : private Graph<std::string> {
private:
    RouteNetwork *route_network_;
    std::unordered_map<std::string ,Location*>* locations_;

    std::unordered_set<Vertex<std::string>*> blockedNodes;
    std::unordered_set<Edge<std::string>*> blockedEdges;

    bool parseLocation(const std::string& location_file);
    bool parseRoute(const std::string& route_file);

public:
    RouteNetwork();
    ~RouteNetwork();
    bool parseData(const std::string& location_data, const std::string& route_data);
    std::unordered_map<std::string ,Location*>* getLocations();
    int getNumberOfLocations();

    std::vector<Vertex<std::string>*> getLocationSet();

    Location* getLocationById(int id);

    void blockNode(int node);
    void blockEdge(int a, int b);
    bool isNodeBlocked(Vertex<std::string>* v);
    bool isEdgeBlocked(Edge<std::string>* e);
    void clearBlocked();

};

#endif //ROUTENETWORK_H
