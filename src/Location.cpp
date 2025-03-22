//
// Created by tiago on 26/02/2025.
//

#include "Location.h"

Location::Location(std::string name, std::string id, std::string code, bool canPark) : Vertex<std::string>(code), name_(name), id_(id), code_(code), canPark_(canPark) {}


std::string Location::getName() {
    return name_;
}

std::string Location::getId() {
    return id_;
}

std::string Location::getCode() {
    return code_;
}

bool Location::getCanPark() {
    return canPark_;
}

void Location::setName(std::string name) {
    name_ = name;
}

void Location::setCode(std::string code) {
    code_ = code;
}

void Location::setCanPark(bool canPark) {
    canPark_ = canPark;
}

void Location::printInfo() {
    std::cout << "Location ID: "<< id_ << std::endl;
    std::cout << "Location Name: "<< name_ << std::endl;
    std::cout << "Location Code: "<< code_ << std::endl;
    if (canPark_) {
        std::cout << "Can park in this location" << std::endl;
    }
    else {
        std::cout << "Can't park in this location" << std::endl;
    }
}

