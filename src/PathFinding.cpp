//
// Created by micael on 28-02-2025.
//


#include "Route.h"
#include "RouteNetwork.h"
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
    for (auto& p : *rn->getLocations()) {
        p.second->setDist(INT_MAX);
        p.second->setPath(nullptr);
        p.second->setProcessing(false);
    }

    Location* org = rn->getLocationById(src_id);
    org->setDist(0);

    MutablePriorityQueue<Vertex<std::string>> pq;
    pq.insert(org);
    org->setProcessing(true);

    while (!pq.empty()) {
        auto u = pq.extractMin();
        u->setProcessing(false);

        for (auto e : u->getAdj()) {
            if (relax(e)) {
                auto* v = e->getDest();
                if (!v->isProcessing()) {
                    pq.insert(v);
                    v->setProcessing(true);
                } else {
                    pq.decreaseKey(v);
                }
            }
        }
    }
}

//maybe make bool for impossible paths
inline void printPath(RouteNetwork *rn, const int &origin, const int &dest) {
    Location *v = rn->getLocationById(dest);
    Location *org = rn->getLocationById(origin);
    std::stack<Location*> s;
    s.push(v);
    double carWeight =0;

    while (v != nullptr && v != org) {
        carWeight += v->getPath()->getDrivingTime();
        v = static_cast<Location*>(v->getPath()->getOrig());
        s.push(v);
    }

    while (!s.empty()) {
        std::cout << s.top()->getId() << " ";
        s.pop();
    }
    std::cout <<"(" << carWeight <<")" << std::endl;
}




