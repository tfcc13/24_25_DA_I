//
// Created by micael on 01-03-2025.
//

#include "RequestProcessor.h"
#include "PathFinding.cpp"

#include <iostream>
#include <ostream>

#define DRIVING_MODE 1
#define WALKING_MODE 0

void RequestProcessor::processRequest(Request &request, RouteNetwork &route_network, int call_mode) {
    if (request.mode == "driving") {
        if (!request.avoidNodes.empty() || !request.avoidSegments.empty() || request.includeNode >= 0) {
            processRestrictedDriving(request, route_network, call_mode);
        }
        else processUnrestrictedDriving(request, route_network, call_mode);
    }
    else if (request.mode == "driving-walking") processDrivingWalking(request, route_network, call_mode);
    else std::cout << "Invalid input format\n";
}

void RequestProcessor::processUnrestrictedDriving(Request &request, RouteNetwork &route_network, int call_mode) {
    std::cout << "Source:" << request.src << std::endl
    << "Destination:" << request.dest << std::endl
    << "BestDrivingRoute:" ;

    double w=0;
    std::vector<Location*> v;

    v = getPath(&route_network, request.src, request.dest, w, DRIVING_MODE);
    printSimplePath(v, w);

    if (v.empty()) return;

    //block intermediate blocks
    for (int i = 1; i < int(v.size())-1; i++) {
        route_network.blockNode(std::stoi(v[i]->getId()));
    }

    std::cout << "AlternativeDrivingRoute:";

    w=0;
    v = getPath(&route_network, request.src, request.dest, w, DRIVING_MODE);
    printSimplePath(v, w);
    route_network.clearBlocked();

}

void RequestProcessor::processRestrictedDriving(Request &request, RouteNetwork &route_network, int call_mode) {
    std::cout << "Source:" << request.src << std::endl
    << "Destination:" << request.dest << std::endl
    << "RestrictedDrivingRoute:" ;

    //update route_network blocked
    if (!request.avoidNodes.empty() || !request.avoidSegments.empty()) {
        for (int id : request.avoidNodes) route_network.blockNode(id);
        for (std::pair<int, int> p : request.avoidSegments) {
            route_network.blockEdge(p.first, p.second);
        }
    }

    double w=0;
    std::vector<Location*> v;
    if (request.includeNode == -1) {
        v = getPath(&route_network, request.src, request.dest, w, DRIVING_MODE);
        printSimplePath(v, w);
        route_network.clearBlocked();
        return;
    }

    route_network.clearBlocked();

    w=0;
    std::vector<Location*> v2;
    v = getPath(&route_network, request.src, request.includeNode, w, DRIVING_MODE);
    v2 = getPath(&route_network, request.includeNode, request.dest, w, DRIVING_MODE);
    if (v.empty() || v2.empty()) {
        std::cout << "none\n";
        return;
    }

    std::vector<Location*> path = mergeIncludePaths(v, v2);
    printSimplePath(path, w);

}



typedef struct {
    double dist;
    std::vector<Location*> path;
} Path;

void RequestProcessor::processDrivingWalking(Request &request, RouteNetwork &route_network, int call_mode) {
    if (!request.avoidNodes.empty() || !request.avoidSegments.empty()) {
        for (int id : request.avoidNodes) route_network.blockNode(id);
        for (std::pair<int, int> p : request.avoidSegments) {
            route_network.blockEdge(p.first, p.second);
        }
    }

    std::unordered_set<Location*> validParkingNodes;
    std::unordered_map<Location*, Path> walkingPath;
    std::unordered_map<Location*, Path> drivingPath;

    dijkstra(&route_network, request.dest, WALKING_MODE);
    for (auto v : route_network.getLocationSet()) {
        auto* l = static_cast<Location*>(v);
        if (l->getCanPark() && l->getDist() < request.maxWalkTime) {
            Path path;
            double w=0;
            path.path = getVectorPath(&route_network, request.dest, std::stoi(l->getId()), w, WALKING_MODE);
            path.dist = w;
            walkingPath[l] = path;
            validParkingNodes.insert(l);
        }

    }

   dijkstra(&route_network, request.src, DRIVING_MODE);
    for (auto p : validParkingNodes) {
        Path path;
        double w=0;
        path.path = getVectorPath(&route_network, request.src, std::stoi(p->getId()), w, DRIVING_MODE);
        path.dist = w;
        drivingPath[p] = path;
    }

    double minTotalTime = INF;
    Location* bestParking = nullptr;

    for (auto p : validParkingNodes) {
        double totalTime = drivingPath[p].dist + walkingPath[p].dist;
        if (totalTime < minTotalTime ||
            ((totalTime == minTotalTime) && walkingPath[p].dist > walkingPath[bestParking].dist)) {
            minTotalTime = totalTime;
            bestParking = p;
        }
    }

    route_network.clearBlocked();

    std::cout << "Source:" << request.src << std::endl
    << "Destination:" << request.dest << std::endl
    << "DrivingRoute:";

    if (bestParking == nullptr) {
        std::cout << "none\n"
        << "ParkingNode:none\n"
        << "WalkingRoot:none\n"
        << "TotalTime:none\n"
        << "Message: No possible route with max. walking time of " << request.maxWalkTime << " minutes.\n";
        return;
    }

    std::vector<Location*> v1 = drivingPath[bestParking].path;
    printSimplePath(v1, drivingPath[bestParking].dist);

    std::cout << "Parking Node:" << bestParking->getId() << std::endl;

    std::cout << "WalkingRoute:";
    std::vector<Location*> v2 = walkingPath[bestParking].path;
    reverse(v2.begin(), v2.end());
    printSimplePath(v2, walkingPath[bestParking].dist);

    std::cout << "TotalTime:" << minTotalTime << std::endl;

}


