//
// Created by micael on 01-03-2025.
//

#ifndef REQUESTPROCESSOR_H
#define REQUESTPROCESSOR_H
#include <string>
#include <utility>
#include <vector>

#include "RouteNetwork.h"

/**
 * @struct Request
 * @brief Represents a routing request with various parameters.
 */

typedef struct {
    std::string mode = "";
    int src=-1;
    int dest=-1;
    std::vector<int> avoidNodes;
    std::vector<std::pair<int, int>> avoidSegments;
    int includeNode = -1;
    int maxWalkTime = -1;
} Request;

/**
 * @class RequestProcessor
 * @brief Processes routing requests.
 */

class RequestProcessor {
public:

    /**
    * @brief Processes a given request and determines the appropriate routing method.
    * @param request The routing request.
    * @param route_network The target route network.
    * @param call_mode The mode of call (ID, Code, Name).
    */
    static void processRequest(Request &request, RouteNetwork &route_network, int call_mode);

    /**
    * @brief Processes a driving route without any restrictions.
    * @param request The routing request.
    * @param route_network The target route network.
    * @param call_mode The mode of call (ID, Code, Name).
    */
    static void processUnrestrictedDriving(Request &request, RouteNetwork &route_network, int call_mode);

    /**
     * @brief Processes a driving route with restrictions (blocked nodes or edges).
     * @param request The routing request.
     * @param route_network The target route network.
     * @param call_mode The mode of call (ID, Code, Name).
     */
    static void processRestrictedDriving(Request &request, RouteNetwork &route_network, int call_mode);

    /**
     * @brief Processes a combined driving and walking route.
     * @param request The routing request.
     * @param route_network The target route network.
     * @param call_mode The mode of call (ID, Code, Name).
     */
    static void processDrivingWalking(Request &request, RouteNetwork &route_network, int call_mode);
};



#endif //REQUESTPROCESSOR_H
