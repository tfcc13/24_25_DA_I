//
// Created by micael on 01-03-2025.
//

#include "RequestProcessor.h"
#include "PathFinding.cpp"
#include <iostream>
#include <ostream>

void RequestProcessor::processRequest(Request &request, RouteNetwork &route_network) {
    if (request.mode == "driving") {
        if (!request.avoidNodes.empty() || !request.avoidSegments.empty() || request.includeNode >= 0) {
            processRestrictedDriving(request, route_network);
        }
        else processUnrestrictedDriving(request, route_network);
    }
    else if (request.mode == "driving-walking") processDrivingWalking(request, route_network);
    else std::cout << "Invalid input format\n";
}

void RequestProcessor::processUnrestrictedDriving(Request &request, RouteNetwork &route_network) {
    std::cout << "Source:" << request.src << std::endl
    << "Destination:" << request.dest << std::endl
    << "BestDrivingRoute:" ;

    int w=0;
    std::vector<std::string> v;

    v = getPath(&route_network, request.src, request.dest, w);
    printSimplePath(v, w);

    //block intermediate blocks
    for (int i = 1; i < int(v.size())-1; i++) {
        route_network.blockNode(std::stoi(v[i]));
    }

    std::cout << "AlternativeDrivingRoute:";

    w=0;
    v = getPath(&route_network, request.src, request.dest, w);
    printSimplePath(v, w);
    route_network.clearBlocked();

}

void RequestProcessor::processRestrictedDriving(Request &request, RouteNetwork &route_network) {
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

    int w=0;
    std::vector<std::string> v;

    if (request.includeNode == -1) {
        v = getPath(&route_network, request.src, request.dest, w);
        printSimplePath(v, w);
        route_network.clearBlocked();
        return;
    }

    w=0;
    std::vector<std::string> v2;
    v = getPath(&route_network, request.src, request.includeNode, w);
    v2 = getPath(&route_network, request.src, request.dest, w);

    //merge v and v2 and then print, remove first val of v2 since its repeated
    //printSimplePath(v3, w);
    route_network.clearBlocked();
}

void RequestProcessor::processDrivingWalking(Request &request, RouteNetwork &route_network) {
    std::cout << "processing walking-driving is not yet available\n";
}


