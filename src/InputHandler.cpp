//
// Created by tiago on 26/02/2025.
//
#include "InputHandler.h"

#include <fstream>
#include <limits>


void InputHandler::waitForInput() {
    std::cout << std::endl << "Press enter to continue ..." << std::endl;
    std::string tempString;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    getline(std::cin,tempString);
}

std::string InputHandler::getInput() {
    std::string input;
    std::cout << ">> ";
    std::cin >> input;
    std::cout << std::endl;
    return input;
}

std::string InputHandler::getInputLine() {
    std::string input;
    std::cout << ">> ";
    getline(std::cin, input);
    std::cout << std::endl;
    return input;
}

std::string InputHandler::toLowerString(const std::string& input) {
    std::string lowerStr = input;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return lowerStr;
}

std::string InputHandler::toUpperString(const std::string& input) {
    std::string lowerStr = input;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return lowerStr;
}

Request InputHandler::parseInputFile(const std::string& filepath) {
    Request route;
    std::ifstream file(filepath);
    std::string line;

    if (!file) {
        std::cerr << "Error: Could not open file " << filepath << "\n";
        return route;
    }

    while(getline(file, line)) {
        std::istringstream iss(line);
        std::string key, value;
        getline(iss >> std::ws, key, ':'); // Remove leading whitespace
        getline(iss >> std::ws, value);    // Remove leading whitespace in value
        value = value.substr(0, value.find("\r"));

        //std::cout << key << " " << value <<std::endl;

        if (key == "Mode") route.mode = value;
        else if (key == "Source") route.src = std::stoi(value);
        else if (key == "Destination") route.dest=std::stoi(value);
        else if (key == "AvoidNodes") {
            std::istringstream ss(value);
            int node;
            while (ss >> node) {
                route.avoidNodes.push_back(node);
                if (ss.peek() == ',') ss.ignore();
            }
        }
        else if (key == "AvoidSegments") {
            std::istringstream ss(value);
            char token;
            int a, b;

            while (ss >> token) {
                if (token != '(') continue;

                ss >> a >> token >> b >> token;

                route.avoidSegments.emplace_back(a,b);
            }
        }
        else if (key == "IncludeNode") {
            if (!value.empty()) route.includeNode = std::stoi(value);
        }
        else if (key == "MaxWalkTime") {
            if (!value.empty()) route.maxWalkTime = std::stoi(value);
        }

    }

    file.close();
    return route;
}
