//
// Created by micael on 01-03-2025.
//

#include "RequestProcessor.h"

#include <fstream>

#include "PathFinding.cpp"

#include <iostream>
#include <ostream>

#include "MultiStream.h"


void RequestProcessor::processRequest(Request &request, RouteNetwork &route_network, int call_mode) {
    std::ofstream outFile("../output/output.txt");
    if (!outFile) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }
    MultiStream out(std::cout, outFile);

    if (request.mode == "driving") {
        if (!request.avoidNodes.empty() || !request.avoidSegments.empty() || request.includeNode >= 0) {
            processRestrictedDriving(request, route_network, call_mode, out);
        }
        else processUnrestrictedDriving(request, route_network, call_mode, out);
    }
    else if (request.mode == "driving-walking") processDrivingWalking(request, route_network, call_mode, out);
    else std::cout << "Invalid input format\n";

    outFile.close();
}

void RequestProcessor::processUnrestrictedDriving(Request &request, RouteNetwork &route_network, int call_mode, MultiStream out) {


    switch (call_mode) {
        case ID_MODE: {
            out << "Source:" << request.src << std::endl
            << "Destination:" << request.dest << std::endl;
            break;
        }
        case CODE_MODE: {
            out << "Source:" << route_network.getLocationById(request.src)->getCode() << std::endl
            << "Destination:" <<  route_network.getLocationById(request.dest)->getCode() << std::endl;
            break;
        }
        case NAME_MODE: {
            out << "Source:" << route_network.getLocationById(request.src)->getName() << std::endl
            << "Destination:" <<  route_network.getLocationById(request.dest)->getName() << std::endl;
            break;
        }
    }

    out << "BestDrivingRoute:" ;


    double w=0;
    std::vector<Location*> v;

    v = getPath(&route_network, request.src, request.dest, w, DRIVING_MODE);
    printSimplePath(v, w, call_mode, out);

    if (v.empty()) return;

    //block intermediate blocks
    for (int i = 1; i < int(v.size())-1; i++) {
        route_network.blockNode(std::stoi(v[i]->getId()));
    }

    out << "AlternativeDrivingRoute:";

    w=0;
    v = getPath(&route_network, request.src, request.dest, w, DRIVING_MODE);
    printSimplePath(v, w, call_mode, out);
    route_network.clearBlocked();

}

void RequestProcessor::processRestrictedDriving(Request &request, RouteNetwork &route_network, int call_mode, MultiStream out) {
    switch (call_mode) {
        case ID_MODE: {
            out << "Source:" << request.src << std::endl
            << "Destination:" << request.dest << std::endl;
            break;
        }
        case CODE_MODE: {
            out << "Source:" << route_network.getLocationById(request.src)->getCode() << std::endl
            << "Destination:" <<  route_network.getLocationById(request.dest)->getCode() << std::endl;
            break;
        }
        case NAME_MODE: {
            out << "Source:" << route_network.getLocationById(request.src)->getName() << std::endl
            << "Destination:" <<  route_network.getLocationById(request.dest)->getName() << std::endl;
            break;
        }
    }
    out<< "RestrictedDrivingRoute:" ;

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
        printSimplePath(v, w, call_mode, out);
        route_network.clearBlocked();
        return;
    }

    route_network.clearBlocked();

    w=0;
    std::vector<Location*> v2;
    v = getPath(&route_network, request.src, request.includeNode, w, DRIVING_MODE);
    v2 = getPath(&route_network, request.includeNode, request.dest, w, DRIVING_MODE);
    if (v.empty() || v2.empty()) {
        out << "none\n";
        return;
    }

    std::vector<Location*> path = mergeIncludePaths(v, v2);
    printSimplePath(path, w, call_mode, out);

}



typedef struct {
    double dist;
    std::vector<Location*> path;
} Path;

void RequestProcessor::processDrivingWalking(Request &request, RouteNetwork &route_network, int call_mode, MultiStream out) {
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
        if (route_network.isNodeBlocked(l)) continue;
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

    switch (call_mode) {
        case ID_MODE: {
            out << "Source:" << request.src << std::endl
            << "Destination:" << request.dest << std::endl;
            break;
        }
        case CODE_MODE: {
            out << "Source:" << route_network.getLocationById(request.src)->getCode() << std::endl
            << "Destination:" <<  route_network.getLocationById(request.dest)->getCode() << std::endl;
            break;
        }
        case NAME_MODE: {
            out << "Source:" << route_network.getLocationById(request.src)->getName() << std::endl
            << "Destination:" <<  route_network.getLocationById(request.dest)->getName() << std::endl;
            break;
        }
    }
    out << "DrivingRoute:";

    if (bestParking == nullptr) {
        out << "none\n"
        << "ParkingNode:none\n"
        << "WalkingRoute:none\n"
        << "TotalTime:none\n"
        << "Message: No possible route with max. walking time of " << request.maxWalkTime << " minutes.\n";
        return;
    }

    std::vector<Location*> v1 = drivingPath[bestParking].path;
    printSimplePath(v1, drivingPath[bestParking].dist, call_mode, out);

    out << "Parking Node:" << bestParking->getId() << std::endl;

    out << "WalkingRoute:";
    std::vector<Location*> v2 = walkingPath[bestParking].path;
    reverse(v2.begin(), v2.end());
    printSimplePath(v2, walkingPath[bestParking].dist, call_mode, out);

    out << "TotalTime:" << minTotalTime << std::endl;

}


