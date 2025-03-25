//
// Created by tiago on 26/02/2025.
//

#ifndef ROUTENETWORK_H
#define ROUTENETWORK_H

#include <unordered_map>
#include <unordered_set>
#include "Graph.h"
#include "Location.h"

#define ID_MODE 0 /**< @brief Mode for using ID in location  */
#define CODE_MODE 1 /**< @brief Mode for using Code in location lookup */
#define NAME_MODE 2  /**< @brief Mode for using Name in location lookup */

#define DRIVING_MODE 1 /**< @brief   Mode for driving route calculation */
#define WALKING_MODE 0 /**< @brief  Mode for walking route calculations */

class RequestProcessor;

class Location;

/**
 * @class RouteNetwork
 * @brief Manages a network of locations connected by routes, allowing for pathfinding and data parsing.
 */
class RouteNetwork : private Graph<std::string> {
private:
    RouteNetwork *route_network_;
    std::unordered_map<std::string ,Location*>* locations_;

    std::unordered_set<Vertex<std::string>*> blockedNodes;
    std::unordered_set<Edge<std::string>*> blockedEdges;

    /**
    * @brief Parses a location data file and loads location data.
    * @param location_file The path to the location file.
    * @return True if parsing was successful, false otherwise.
    */
    bool parseLocation(const std::string& location_file);

    /**
     * @brief Parses a route data file and loads route data.
     * @param route_file The path to the route file.
     * @return True if parsing was successful, false otherwise.
     */
    bool parseRoute(const std::string& route_file);

public:
    /**
     * @brief Constructs a new RouteNetwork object.
     */
    RouteNetwork();

    /**
    * @brief Destroys the RouteNetwork object and frees allocated memory.
    */
    ~RouteNetwork();

    /**
     * @brief Parses location and route data from files.
     * @param location_data Path to the location data file.
     * @param route_data Path to the route data file.
     * @return True if both files were successfully parsed, false otherwise.
     */
    bool parseData(const std::string& location_data, const std::string& route_data);

    /**
     * @brief Retrieves the locations stored in an unordered map data structure .
     * @return Pointer to the unordered map of locations.
     */
    std::unordered_map<std::string ,Location*>* getLocations();

    /**
    * @brief Gets the number of locations in the network.
    * @return The number of locations.
    */
    int getNumberOfLocations();

    /**
     * @brief Retrieves all location vertices in the network.
     * @return A vector of location vertices.
     */
    std::vector<Vertex<std::string>*> getLocationSet();

    /**
    * @brief Finds a location by its numeric ID.
    * @param id The location ID.
    * @return Pointer to the Location object or nullptr if not found.
    */
    Location* getLocationById(int id);

    /**
     * @brief Finds a location by its unique code.
     * @param code The location code.
     * @return Pointer to the Location object or nullptr if not found.
     */
    Location* getLocationByCode(const std::string& code);

    /**
     * @brief Finds a location by its name.
     * @param name The location name.
     * @return Pointer to the Location object or nullptr if not found.
     */
    Location* getLocationByName(const std::string& name);

    /**
     * @brief Blocks a specific node in the network and adds it to an unordered set structure.
     * @param node The ID of the node to block.
     */
    void blockNode(int node);

    /**
     * @brief Blocks an edge between two locations and adds it to an unordered set structure.
     * @param a First location ID.
     * @param b Second location ID.
     */
    void blockEdge(int a, int b);

    /**
     * @brief Checks if a node is blocked.
     * @param v The vertex to check.
     * @return True if the node is blocked, false otherwise.
     */
    bool isNodeBlocked(Vertex<std::string>* v);

    /**
     * @brief Checks if an edge is blocked.
     * @param e The edge to check.
     * @return True if the edge is blocked, false otherwise.
     */
    bool isEdgeBlocked(Edge<std::string>* e);

    /**
     * @brief Clears all blocked nodes and edges from the unordered set structures.
     */
    void clearBlocked();

    /**
     * @brief Displays information about a location by its ID.
     * @param id The location ID.
     */
    void showLocationInfoById(std::string const&  id);

    /**
     * @brief Displays information about a location by its code.
     * @param code The location code.
     */
    void showLocationInfoByCode(std::string const&  code);

    /**
     * @brief Displays information about a location by its name.
     * @param name The location name.
     */
    void showLocationInfoByName(std::string const&  name);

    /**
     * @brief Finds and displays the optimal route between two locations by ID.
     * @param src Source location ID.
     * @param dest Destination location ID.
     * @param route_network The RouteNetwork object for route calculation.
     * @param call_mode The mode (ID, Code, Name).
     * @param route_mode The route mode (Driving or Walking).
     * @param max_walk_time (Optional) Maximum allowed walking time restriction.
     */
    void routeById(int src, int dest, RouteNetwork& route_network, int call_mode, int route_mode, int max_walk_time =-1);
    void routeByCode(const std::string &src, const std::string &dest, RouteNetwork& route_network, int route_mode,int max_walk_time =-1);
    void routeByName(const std::string &src, const std::string &dest, RouteNetwork& route_network, int route_mode, int max_walk_time =-1);


};

#endif //ROUTENETWORK_H
