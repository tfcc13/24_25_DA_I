#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <vector>
#include "Route.h"
#include "RouteNetwork.h"
#include "../data_structures/MutablePriorityQueue.h"
#include <stack>
#include <sstream>
#include "MultiStream.h"

/**
 * @class PathFinding.h
 * @brief Provides pathfinding algorithms for route networks.
 */

class PathFinding {
    public:

    /**
    * @brief Relaxes an edge in the shortest path algorithm.
    * @param route Pointer to the edge being evaluated.
    * @param isDriving Boolean flag indicating whether to use driving time or walking time.
    * @return True if the relaxation was successful, otherwise false.
    */
    static bool relax(Edge<std::string>* route, bool isDriving);

    /**
    * @brief Runs Dijkstra's algorithm on the given RouteNetwork.
    * @details **Time Complexity** O((V+E)logV)
    * @param rn Pointer to the RouteNetwork.
    * @param src_id The ID of the source location.
    * @param mode Boolean flag indicating whether to use driving mode (true) or walking mode (false).
    */
    static void dijkstra(RouteNetwork* rn, int src_id, bool mode);

    /**
    * @brief Retrieves the shortest path as a vector of locations.
    * @param rn Pointer to the RouteNetwork.
    * @param origin The origin location ID.
    * @param dest The destination location ID.
    * @param weight Reference to a double storing the total path weight.
    * @param isDriving Boolean flag indicating whether to use driving time (true) or walking time (false).
    * @return A vector containing the locations that form the shortest path.
    */
    static std::vector<Location*> getVectorPath(RouteNetwork *rn, const int &origin, const int &dest, double &weight, bool isDriving);

    /**
    * @brief Prints a simple representation of the path.
    * @param v The vector of locations representing the path.
    * @param weight The total weight of the path.
    * @param call_mode The mode used for displaying location information (ID, Code, or Name).
    */
    static void printSimplePath(std::vector<Location*> v, double weight, int call_mode, MultiStream out);

    /**
    * @brief Computes the shortest path between two locations.
    * @param rn Pointer to the RouteNetwork.
    * @param source The source location ID.
    * @param dest The destination location ID.
    * @param weight Reference to a double storing the total path weight.
    * @param mode Boolean flag indicating whether to use driving mode (true) or walking mode (false).
    * @return A vector containing the locations that form the shortest path.
    */
    static std::vector<Location*> getPath(RouteNetwork *rn, int source, int dest, double &weight, bool mode);

    /**
    * @brief Merges two paths, ensuring the second path continues from the first.
    * @param v1 The first path as a vector of locations.
    * @param v2 The second path as a vector of locations.
    * @return A merged vector containing the combined path.
    */
    static std::vector<Location*> mergeIncludePaths(std::vector<Location*> v1, std::vector<Location*> v2);




};

#endif //PATHFINDING_H
