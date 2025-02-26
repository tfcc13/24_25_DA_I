#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <limits.h>
#include "../data_structures/Graph.h"

void readLocations(Graph<std::string> *graph) {
    std::ifstream file("csv/locEx.csv");

    std::string line;

    if (std::getline(file, line)) {  //ignore header
    }

    while(std::getline(file, line)) {
        std::stringstream ss(line);
        std::string name, idStr, code, canParkStr;

        std::getline(ss, name, ',');
        std::getline(ss, idStr, ',');
        std::getline(ss, code, ',');
        std::getline(ss, canParkStr, ',');

        bool canPark = (std::stoi(canParkStr) == 1);

        std::cout << name << " " << idStr << " " << code << " " << canPark <<std::endl;

        graph->addVertex(code, std::stoi(idStr), name, canPark);
    }
}

void readDistances(Graph<std::string> *graph) {
    std::ifstream file("csv/distEx.csv");
    std::string line;

    if (std::getline(file, line))  //ignore header
    

    while(std::getline(file, line)) {
        std::stringstream ss(line);
        std::string code1, code2, carDistStr, bikeDistStr;

        std::getline(ss, code1, ',');
        std::getline(ss, code2, ',');
        std::getline(ss, carDistStr, ',');
        std::getline(ss, bikeDistStr, ',');

        int carDist;
        if (carDistStr == "X") carDist = INT_MAX;
        else carDist = std::stoi(carDistStr);
        int bikeDist = std::stoi(bikeDistStr);

        std::cout << code1 << " " << code2 << " " << carDist << " " << bikeDist <<" "  <<std::endl;


        graph->addBidirectionalEdge(code1, code2, carDist, bikeDist);
    }
}    

Graph<std::string> buildGraph() {
    Graph<std::string> g;

    readLocations(&g);
    readDistances(&g);

    return g;
}