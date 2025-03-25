//
// Created by tiago on 26/02/2025.
//

#include "RouteNetwork.h"

#include <fstream>
#include <sstream>

#include "InputHandler.h"
#include "Route.h"

RouteNetwork::RouteNetwork() {
    route_network_ = nullptr;
    locations_ = new std::unordered_map<std::string, Location*>();
}

RouteNetwork::~RouteNetwork() {
    for (auto& pair : *locations_) {
        delete pair.second;
    }
    delete route_network_;
    delete locations_;

}

// change this
void trimString(std::string &str) {
    while (!str.empty() && (str.back() == '\r' || str.back() == ','))
        str.pop_back();
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
        getline(iss,location,sep);
        getline(iss,id,sep);
        getline(iss,code,sep);
        getline(iss,park,sep);
        trimString(park);

        canPark = (std::stoi(park)  == 1);

        auto* tempLocation = new Location(location,id,code,canPark);
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
        trimString(walking_time);

        if (driving_time == "X") {
            d_time = INT_MAX;
        }
        else {
            d_time = std::stod(driving_time);
        }
        w_time = std::stod(walking_time);

        addBidirectionalEdge(orig_location,dest_location,w_time,d_time);
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

std::unordered_map<std::string ,Location*>* RouteNetwork::getLocations() {
    return locations_;
}

int RouteNetwork::getNumberOfLocations() {
    return this->getVertexSet().size();
}

Location* RouteNetwork::getLocationById(int id) {
    for (auto loc : *locations_) {
        if (std::stoi(loc.second->getId()) == id) return loc.second;
    }
    return nullptr;
}

Location* RouteNetwork::getLocationByCode(const std::string& code) {

    std::string upperCode = InputHandler::toUpperString(code);

    auto iter = locations_->find(upperCode);
    if (iter == locations_->end()) {
        return nullptr;
    }
    return iter->second;
}

Location* RouteNetwork::getLocationByName(const std::string& name) {

    std::string lowerName = InputHandler::toLowerString(name);

    for (auto loc : *locations_) {
        if (InputHandler::toLowerString(loc.second->getName()) == lowerName) return loc.second;
    }
    return nullptr;
}


std::vector<Vertex<std::string>*> RouteNetwork::getLocationSet() {
    return this->getVertexSet();
}

void RouteNetwork::blockNode(int id) {
    Location* l = getLocationById(id);
    blockedNodes.insert(l);
}

void RouteNetwork::blockEdge(int id1, int id2) {
    Vertex<std::string>* l1 = getLocationById(id1);
    Vertex<std::string>* l2 = getLocationById(id2);

    for (auto e : l1->getAdj()) {
        auto w = e->getDest();
        if (l2 == w) {
            blockedEdges.insert(e);
        }
    }

    for (auto e : l2->getAdj()) {
        auto w = e->getDest();
        if (l1 == w) {
            blockedEdges.insert(e);
        }

    }
}

bool RouteNetwork::isNodeBlocked(Vertex<std::string>* v) {
    return blockedNodes.find(v) != blockedNodes.end();
}

bool RouteNetwork::isEdgeBlocked(Edge<std::string>* e) {
    return blockedEdges.find(e) != blockedEdges.end();
}

void RouteNetwork::clearBlocked() {
    blockedNodes.clear();
    blockedEdges.clear();
}

void RouteNetwork::showLocationInfoById(std::string const&  id) {
    Location* loc = getLocationById(std::stoi(id));
    if ( loc == nullptr) {
        std::cout << "Location " << id << " does not exist" << std::endl;
    }
    else {
        loc->printInfo();
    }
}

void RouteNetwork::showLocationInfoByCode(std::string const&  code) {
    Location* loc = getLocationByCode(code);
    if ( loc == nullptr) {
        std::cout << "Location " << code << " does not exist" << std::endl;
    }
    else {
        loc->printInfo();
    }
}

void RouteNetwork::showLocationInfoByName(std::string const&  name) {
    Location* loc = getLocationByName(name);
    if ( loc == nullptr) {
        std::cout << "Location " << name << " does not exist" << std::endl;
    }
    else {
        loc->printInfo();
    }
}

void RouteNetwork::routeById(const std::string &src, const std::string &dest, RouteNetwork& route_network, int call_mode, int route_mode, int max_walk_time) {
    int src_id = std::stoi(src);
    int dest_id = std::stoi(dest);
    if (getLocationById(src_id) == nullptr  || getLocationById(dest_id) == nullptr) {
        std::cout << "Source " << src_id << " or destination " << dest_id << " doesn't exist." << std::endl;
        return;
    }

    Request request;

    if (route_mode == DRIVING_MODE) {
        request.mode = "driving";
    }
    else {
        request.mode = "driving-walking";
    }

    request.maxWalkTime = max_walk_time;
    request.src = src_id;
    request.dest = dest_id;

    RequestProcessor::processRequest(request, route_network, call_mode );

}

void RouteNetwork::routeByCode(const std::string &src, const std::string &dest, RouteNetwork& route_network, int route_mode, int max_walk_time) {
    auto src_loc = getLocationByCode(src);
    auto dest_loc = getLocationByCode(dest);

    if (src_loc == nullptr  || dest_loc == nullptr) {
        std::cout << "Source " << src << " or destination " << dest << " doesn't exist." << std::endl;
        return;
    }

    routeById(src_loc->getId(), dest_loc->getId(), route_network, CODE_MODE, route_mode, max_walk_time);

}

void RouteNetwork::routeByName(const std::string &src, const std::string &dest, RouteNetwork& route_network, int route_mode, int max_walk_time) {
    auto src_loc = getLocationByName(src);
    auto dest_loc = getLocationByName(dest);

    if (src_loc == nullptr  || dest_loc == nullptr) {
        std::cout << "Source " << src << " or destination " << dest << " doesn't exist." << std::endl;
        return;
    }

    routeById(src_loc->getId(), dest_loc->getId(), route_network, NAME_MODE, route_mode, max_walk_time);

}

