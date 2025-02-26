//
// Created by tiago on 26/02/2025.
//
#include "InputHandler.h"

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
