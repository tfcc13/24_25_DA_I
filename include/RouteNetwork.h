//
// Created by tiago on 26/02/2025.
//

#ifndef ROUTENETWORK_H
#define ROUTENETWORK_H

#include <unordered_map>
#include <unordered_set>
#include "Graph.h"
#include "Location.h"

#define ID_MODE 0
#define CODE_MODE 1
#define NAME_MODE 2

#define DRIVING_MODE 1
#define WALKING_MODE 0

class RequestProcessor;

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
    Location* getLocationByCode(const std::string& code);
    Location* getLocationByName(const std::string& name);

    void blockNode(int node);
    void blockEdge(int a, int b);
    bool isNodeBlocked(Vertex<std::string>* v);
    bool isEdgeBlocked(Edge<std::string>* e);
    void clearBlocked();
    void showLocationInfoById(std::string const&  id);
    void showLocationInfoByCode(std::string const&  code);
    void showLocationInfoByName(std::string const&  name);
    void routeById(const std::string &src, const std::string &dest, RouteNetwork& route_network, int call_mode, int route_mode, int max_walk_time =-1);
    void routeByCode(const std::string &src, const std::string &dest, RouteNetwork& route_network, int route_mode,int max_walk_time =-1);
    void routeByName(const std::string &src, const std::string &dest, RouteNetwork& route_network, int route_mode, int max_walk_time =-1);


};

#endif //ROUTENETWORK_H
