#ifndef REQUESTPROCESSOR_H
#define REQUESTPROCESSOR_H
#include <string>
#include <utility>
#include <vector>

#include "MultiStream.h"
#include "RouteNetwork.h"

/**
 * @struct Request
 * @brief Represents a routing request with various parameters.
 */

typedef struct {
    std::string mode = ""; /**< Travel mode (e.g., "driving", "driving-walking"). */
    int src=-1; /**< Source location ID. */
    int dest=-1; /**< Destination location ID. */
    std::vector<int> avoidNodes; /**< Nodes to avoid in routing. */
    std::vector<std::pair<int, int>> avoidSegments; /**< Edges to avoid in routing. */
    int includeNode = -1; /**< A mandatory intermediate node in the route. */
    int maxWalkTime = -1; /**< Maximum allowed walking time (for mixed modes). */
} Request;

/**
 * @class RequestProcessor
 * @brief Processes routing requests.
 */

class RequestProcessor {
public:

    /**
    * @brief Processes a given request and determines the appropriate routing method.
    * @details O((V + E) log V) for pathfinding operations.
    * @param request The routing request.
    * @param route_network The target route network.
    * @param call_mode The mode of call (ID, Code, Name).
    */
    static void processRequest(Request &request, RouteNetwork &route_network, int call_mode);

    /**
    * @brief Processes a driving route without any restrictions.
    * @details O((V + E) log V) due to Dijkstra’s algorithm.
    * @param request The routing request.
    * @param route_network The target route network.
    * @param call_mode The mode of call (ID, Code, Name).
    * @param out Output stream handler.
    */
    static void processUnrestrictedDriving(Request &request, RouteNetwork &route_network, int call_mode, MultiStream out);

    /**
    * @brief Processes a driving route with restrictions (blocked nodes or edges).
    * @details O((V + E) log V), considering blocked nodes and alternative paths.
    * @param request The routing request.
    * @param route_network The target route network.
    * @param call_mode The mode of call (ID, Code, Name).
    * @param out Output stream handler.
    */
    static void processRestrictedDriving(Request &request, RouteNetwork &route_network, int call_mode, MultiStream out);

    /**
    * @brief Processes a combined driving and walking route.
    * @details O((V + E) log V) for Dijkstra’s calculations.
    * @param request The routing request.
    * @param route_network The target route network.
    * @param call_mode The mode of call (ID, Code, Name).
    * @param out Output stream handler.
    */
    static void processDrivingWalking(Request &request, RouteNetwork &route_network, int call_mode, MultiStream out);
};



#endif //REQUESTPROCESSOR_H
