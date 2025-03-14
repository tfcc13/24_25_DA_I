//
// Created by micael on 28-02-2025.
//


#include "Route.h"
#include "RouteNetwork.h"
#include "RequestProcessor.h"
#include "../data_structures/MutablePriorityQueue.h"
#include <stack>

inline bool relax(Edge<std::string>* route) {
    auto u = route->getOrig();
    auto v = route->getDest();
    if (v->getDist() > u->getDist() + route->getDrivingTime()) {
        v->setDist(u->getDist() + route->getDrivingTime());
        v->setPath(route);
        return true;
    }
    return false;

}

inline void dijkstra(RouteNetwork* rn, int src_id) {
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

            if (relax(e)) {
                auto* v = e->getDest();
                pq.decreaseKey(v);

            }
        }
    }
}

inline std::vector<std::string> getVectorPath(RouteNetwork *rn, const int &origin, const int &dest, int &weight) {
    Location *v = rn->getLocationById(dest);
    Location *org = rn->getLocationById(origin);
    std::vector<std::string> path;
    std::stack<Location*> s;
    s.push(v);
    if (v->getPath() == nullptr) return path;

    while (v != nullptr && v != org) {
        weight += v->getPath()->getDrivingTime();
        v = static_cast<Location*>(v->getPath()->getOrig());
        s.push(v);
    }

    while (!s.empty()) {
        path.push_back(s.top()->getId());
        s.pop();
    }
    return path;
}


inline void printSimplePath(std::vector<std::string> v, int weight) {
    if (v.empty()) {
        std::cout << "None\n";
        return;
    }
    for (auto s : v) std::cout << s <<",";
    std::cout << "(" << weight << ")\n";
}

inline std::vector<std::string> getPath(RouteNetwork *rn, int source, int dest, int &weight) {
    dijkstra(rn, source);
    return getVectorPath(rn, source, dest, weight);
}




