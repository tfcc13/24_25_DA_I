//
// Created by micael on 01-03-2025.
//

#include "RequestProcessor.h"
#include "PathFinding.cpp"

#include <iostream>
#include <ostream>

#define DRIVING_MODE 1
#define WALKING_MODE 0

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

    v = getPath(&route_network, request.src, request.dest, w, DRIVING_MODE);
    printSimplePath(v, w);

    //block intermediate blocks
    for (int i = 1; i < int(v.size())-1; i++) {
        route_network.blockNode(std::stoi(v[i]));
    }

    std::cout << "AlternativeDrivingRoute:";

    w=0;
    v = getPath(&route_network, request.src, request.dest, w, DRIVING_MODE);
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
        v = getPath(&route_network, request.src, request.dest, w, DRIVING_MODE);
        printSimplePath(v, w);
        route_network.clearBlocked();
        return;
    }

    w=0;
    std::vector<std::string> v2;
    v = getPath(&route_network, request.src, request.includeNode, w, DRIVING_MODE);
    v2 = getPath(&route_network, request.includeNode, request.dest, w, DRIVING_MODE);
    if (v.empty() || v2.empty()) {
        std::cout << "none\n";
        return;
    }

    std::vector<std::string> path = mergeIncludePaths(v, v2);
    printSimplePath(path, w);

    route_network.clearBlocked();
}

void RequestProcessor::processDrivingWalking(Request &request, RouteNetwork &route_network) {
    std::cout << "processing walking-driving is not yet available\n";

    //can be done with 4 dijkstras, essentially the same as include node in restricted

    //do one from dest in WALKING_MODE, and get the parkable nodes within max_walk_dist, store in set and map(specific for WALKING) that has dist for walking
    //do one from src in DRIVING_MODE, store the drive dist in map for the nodes that are int the set of park nodes

    // iterate through set of parked nodes, find optimal parking node. the one with lower driving + walking, O(1) search with map

    //getpath from src to park_node, DRIVING_MODE
    //getpath from park_node to dest, WALKING_MODE
    //merge path and print






}


