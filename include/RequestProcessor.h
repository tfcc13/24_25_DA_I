//
// Created by micael on 01-03-2025.
//

#ifndef REQUESTPROCESSOR_H
#define REQUESTPROCESSOR_H
#include <string>
#include <utility>
#include <vector>

#include "RouteNetwork.h"


typedef struct {
    std::string mode = "";
    int src=-1;
    int dest=-1;
    std::vector<int> avoidNodes;
    std::vector<std::pair<int, int>> avoidSegments;
    int includeNode = -1;
    int maxWalkTime = -1;
} Request;


class RequestProcessor {
public:
    static void processRequest(Request &request, RouteNetwork &route_network);
    static void processUnrestrictedDriving(Request &request, RouteNetwork &route_network);
    static void processRestrictedDriving(Request &request, RouteNetwork &route_network);
    static void processDrivingWalking(Request &request, RouteNetwork &route_network);
};



#endif //REQUESTPROCESSOR_H
