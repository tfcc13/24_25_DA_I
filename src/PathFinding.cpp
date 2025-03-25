//
// Created by micael on 28-02-2025.
//


#include "Route.h"
#include "RouteNetwork.h"
#include "RequestProcessor.h"
#include "../data_structures/MutablePriorityQueue.h"
#include <stack>
#include <sstream>

/**
* @brief Relaxes an edge in the shortest path algorithm.
* @param route Pointer to the edge being evaluated.
* @param isDriving Boolean flag indicating whether to use driving time or walking time.
* @return True if the relaxation was successful, otherwise false.
*/
inline bool relax(Edge<std::string>* route, bool isDriving) {
    auto u = route->getOrig();
    auto v = route->getDest();
    if (isDriving) {
        if (v->getDist() > u->getDist() + route->getDrivingTime()) {
            v->setDist(u->getDist() + route->getDrivingTime());
            v->setPath(route);
            return true;
        }
    }
    else {
        if (v->getDist() > u->getDist() + route->getWalkingTime()) {
            v->setDist(u->getDist() + route->getWalkingTime());
            v->setPath(route);
            return true;
        }
    }
    return false;

}

/**
* @brief Runs Dijkstra's algorithm on the given RouteNetwork.
* @param rn Pointer to the RouteNetwork.
* @param src_id The ID of the source location.
* @param mode Boolean flag indicating whether to use driving mode (true) or walking mode (false).
*/
inline void dijkstra(RouteNetwork* rn, int src_id, bool mode) {
    for (auto& p : rn->getLocationSet() ){
        p->setDist(INT_MAX);
        p->setPath(nullptr);
    }

    Location* org = rn->getLocationById(src_id);
    org->setDist(0);
    MutablePriorityQueue<Vertex<std::string>> pq;

    for (auto v: rn->getLocationSet()) {
        pq.insert(v);
    }

    while (!pq.empty()) {
        auto u = pq.extractMin();
        if (rn->isNodeBlocked(u)) continue;

        for (auto e : u->getAdj()) {
            if (rn->isEdgeBlocked(e)) continue;

            if (relax(e, mode)) {
                auto* v = e->getDest();
                pq.decreaseKey(v);

            }
        }
    }
}

/**
* @brief Retrieves the shortest path as a vector of locations.
* @param rn Pointer to the RouteNetwork.
* @param origin The origin location ID.
* @param dest The destination location ID.
* @param weight Reference to a double storing the total path weight.
* @param isDriving Boolean flag indicating whether to use driving time (true) or walking time (false).
* @return A vector containing the locations that form the shortest path.
*/
inline std::vector<Location*> getVectorPath(RouteNetwork *rn, const int &origin, const int &dest, double &weight, bool isDriving) {
    Location *v = rn->getLocationById(dest);
    Location *org = rn->getLocationById(origin);
    std::vector<Location*> path;
    std::stack<Location*> s;
    s.push(v);
    if (v->getPath() == nullptr) return path;

    while (v != nullptr && v != org) {
        if (isDriving) weight += v->getPath()->getDrivingTime();
        else weight += v->getPath()->getWalkingTime();

        v = static_cast<Location*>(v->getPath()->getOrig());
        s.push(v);
    }

    while (!s.empty()) {
        path.push_back(s.top());
        s.pop();
    }
    return path;
}

/**
* @brief Prints a simple representation of the path.
* @param v The vector of locations representing the path.
* @param weight The total weight of the path.
* @param call_mode The mode used for displaying location information (ID, Code, or Name).
*/
inline void printSimplePath(std::vector<Location*> v, double weight, int call_mode) {
    if (v.empty()) {
        std::cout << "none\n";
        return;
    }

    std::ostringstream output;


    for (auto s : v) {
        switch (call_mode) {
            case ID_MODE: {
                output << s->getId() <<",";
                break;
            }
            case CODE_MODE: {
                output << s->getCode() <<",";
                break;
            }
            case NAME_MODE: {
                output << s->getName() <<",";
                break;
            }
        }

    }

    std::string result = output.str();
    if (!result.empty()) {
        result.pop_back();  // Remove the last comma
    }
    std::cout << result << "(" << weight << ")\n";


}

/**
* @brief Computes the shortest path between two locations.
* @param rn Pointer to the RouteNetwork.
* @param source The source location ID.
* @param dest The destination location ID.
* @param weight Reference to a double storing the total path weight.
* @param mode Boolean flag indicating whether to use driving mode (true) or walking mode (false).
* @return A vector containing the locations that form the shortest path.
*/
inline std::vector<Location*> getPath(RouteNetwork *rn, int source, int dest, double &weight, bool mode) {
    dijkstra(rn, source, mode);
    return getVectorPath(rn, source, dest, weight, mode);
}


/**
* @brief Merges two paths, ensuring the second path continues from the first.
* @param v1 The first path as a vector of locations.
* @param v2 The second path as a vector of locations.
* @return A merged vector containing the combined path.
*/
inline std::vector<Location*> mergeIncludePaths(std::vector<Location*> v1, std::vector<Location*> v2) {
    std::vector<Location*> path;
    for (auto s : v1) path.push_back(s);
    for (size_t i = 1; i < v2.size(); i++) path.push_back(v2[i]);
    return path;
}



