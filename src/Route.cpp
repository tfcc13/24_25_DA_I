//
// Created by tiago on 26/02/2025.
//

#include "Route.h"

Route::Route(Vertex<std::string> *orig, Vertex<std::string> *dest, double walking_time, double driving_time): Edge(orig, dest, walking_time, driving_time) {
    orig_ = orig->getInfo();
    dest_ = dest->getInfo();
    walking_time_ = walking_time;
    driving_time_ = driving_time;
}

std::string Route::get_orig() {
    return orig_;
}

std::string Route::get_dest() {
    return dest_;
}


double Route::get_walking_time() {
    return walking_time_;
}

double Route::get_driving_time() {
    return driving_time_;
}


