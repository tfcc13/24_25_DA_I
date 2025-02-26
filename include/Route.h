//
// Created by tiago on 26/02/2025.
//

#ifndef ROUTE_H
#define ROUTE_H
#include <Graph.h>

class Route : public Edge<std::string> {
private:
    double walking_time_;
    double driving_time_;
    std::string orig_;
    std::string dest_;

public:
    Route(Vertex<std::string>* orig, Vertex<std::string>* dest, double walking_time, double driving_time);
    double get_walking_time();
    double get_driving_time();
    std::string get_orig();
    std::string get_dest();

};

#endif //ROUTE_H
