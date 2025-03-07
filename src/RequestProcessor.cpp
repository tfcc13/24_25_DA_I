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
    dijkstra(&route_network, request.src);

    int w=0;
    std::vector<std::string> v = getPath(&route_network, request.src, request.dest, w);
    if (v.empty()) {
        std::cout << "None\n";
        return;
    }

    for (auto s : v) std::cout << s <<" ";
    std::cout << std::endl;
    //alternative route to be implemented, just remove the used nodes and repeat
}

void RequestProcessor::processRestrictedDriving(Request &request, RouteNetwork &route_network) {
    std::cout << "processing restricted driving is not yet available\n";
}

void RequestProcessor::processDrivingWalking(Request &request, RouteNetwork &route_network) {
    std::cout << "processing walking-driving is not yet available\n";
}


