//
// Created by tiago on 26/02/2025.
//

#include "Menu.h"
#include "../src/PathFinding.cpp"
#include <filesystem>

#include "RequestProcessor.h"

Menu::Menu(RouteNetwork &route_network) : route_network_(route_network) {}

Menu *Menu::invalidInput() {
    std::cout << "Invalid option. Please try again." << std::endl;
    InputHandler::waitForInput();
    return this;
}

DataMenu::DataMenu(RouteNetwork& network) : Menu(network) {}

void DataMenu::show() {
    std::cout << CLEAR;
    int options = 0;
    std::cout << R"(
     //////////////////////////////////////////////////////////////
     //  _____ _   _  ____    _      __  __    _    ____  ____   //
     // |_   _| | | |/ ___|  / \    |  \/  |  / \  |  _ \/ ___|  //
     //   | | | | | | |  _  / _ \   | |\/| | / _ \ | |_) \___ \  //
     //   | | | |_| | |_| |/ ___ \  | |  | |/ ___ \|  __/ ___) | //
     //   |_|  \___/ \____/_/   \_\ |_|  |_/_/   \_\_|   |____/  //
     //////////////////////////////////////////////////////////////
    )" <<  "\n\n";


    std::cout << "MENU: \n\n";

    std::cout << "(" << ++options << ") >> " << "Read Small Dataset" << std::endl;
    std::cout << "(" << ++options << ") >> " << "Read Large Datase" << std::endl;
    std::cout << "(0) >> Exit "<< std::endl;

}

Menu *DataMenu::getNextMenu() {
    int option;
    if(!InputHandler::get(option)) {
        return invalidInput();
    }
    switch (option) {
        case 0: {
            return nullptr;
        }
        case 1: {
            route_network_.parseData("../dataset/locEx.csv","../dataset/distEx.csv");
            break;
        }
        case 2: {
            route_network_.parseData("../dataset/Locations.csv","../dataset/Distances.csv");
            break;
        }
    }

    return new MainMenu(route_network_);
}

MainMenu::MainMenu(RouteNetwork& network) : Menu(network) {}

void MainMenu::show() {
    std::cout << CLEAR;
    int options = 0;
    std::cout << R"(
     //////////////////////////////////////////////////////////////
     //  _____ _   _  ____    _      __  __    _    ____  ____   //
     // |_   _| | | |/ ___|  / \    |  \/  |  / \  |  _ \/ ___|  //
     //   | | | | | | |  _  / _ \   | |\/| | / _ \ | |_) \___ \  //
     //   | | | |_| | |_| |/ ___ \  | |  | |/ ___ \|  __/ ___) | //
     //   |_|  \___/ \____/_/   \_\ |_|  |_/_/   \_\_|   |____/  //
     //////////////////////////////////////////////////////////////
    )" <<  "\n\n";


    std::cout << "MENU: \n\n";

    std::cout << "(" << ++options << ") >> " << "Go to Locations  Menu" << std::endl;
    std::cout << "(" << ++options << ") >> " << "Go to Routes Menu" << std::endl;
    std::cout << "(" << ++options << ") >> " << "Process Input" << std::endl;
    std::cout << "(0) >> Go back "  << "Exit" << std::endl;
}

Menu *MainMenu::getNextMenu() {
    int option;
    if(!InputHandler::get(option)) {
        return invalidInput();
    }
    switch (option) {
        case 0: {
            return nullptr;
        }
        case 1: {
            return new LocationMenu(route_network_);
        }
        case 2: {
            return  new RouteMenu(route_network_);
            break;
        }
        case 3: {
            return new InputMenu(route_network_);
        }
    }
    InputHandler::waitForInput();

    return this;
}

LocationMenu::LocationMenu(RouteNetwork& network) : Menu(network) {}
void LocationMenu::show() {
    std::cout << CLEAR;
    int options = 0;
    std::cout << "(" << ++options << ") >> " << "Check the size of the network" << std::endl;
    std::cout << "(" << ++options << ") >> " << "Check all the locations of the network" << std::endl;
    std::cout << "(" << ++options << ") >> " << "Check a Location Info" << std::endl;
    std::cout << "(0) >> Go back "<< std::endl;
}

Menu *LocationMenu::getNextMenu() {
    int option;
    if(!InputHandler::get(option)) {
        return invalidInput();
    }

    switch (option) {
        case 0: {
            return nullptr;
        }
        case 1: {
            //int map_size = route_network_.getLocations()->size();
            int map_size = route_network_.getNumberOfLocations();
            std::cout << "The network has " << map_size << " locations." << std::endl;
            break;
        }
        case 2: {
            for (const auto& location : *route_network_.getLocations()) {
                std::cout << location.second->getCode() << " " << location.second->getName() << " " << location.second->getAdj().size() <<  std::endl;
            }
            break;
        }
        case 3: {
            return new LocationInfoMenu(route_network_);
        }
    }

    InputHandler::waitForInput();

    return this;
}




InputMenu::InputMenu(RouteNetwork &network) : Menu(network) {}

void InputMenu::show() {
    std::cout << CLEAR;
    int options = 0;

    std::cout << "CHOSE THE INPUT FILE: \n\n";
    std::string directory = "../input/";
    std::vector<std::filesystem::path> files;

    try {
        for (const auto &entry : std::filesystem::directory_iterator(directory)) {
            if (entry.is_regular_file()) {
                files.push_back(entry.path().filename());
            }
        }

        std::sort(files.begin(), files.end());

        for (const auto &file : files) {
            std::cout << "(" << ++options << ") >> " << file.string() << std::endl;
        }
    } catch (const std::filesystem::filesystem_error &e) {
        std::cout << e.code() << " " << e.what() << std::endl;
    }


    std::cout << "(0) >> Go back "<< std::endl;
}



Menu *InputMenu::getNextMenu() {
    int option;
    if(!InputHandler::get(option)) {
        return invalidInput();
    }

    std::string directory = "../input/";
    std::vector<std::filesystem::path> files;

    for (const auto &entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            files.push_back(entry.path().filename());
        }
    }
    std::sort(files.begin(), files.end());

    if (option == 0) return nullptr;

    std::string filepath = directory + files[option - 1].string();
    Request r = InputHandler::parseInputFile(filepath);
    RequestProcessor::processRequest(r, route_network_);

    InputHandler::waitForInput();

    return this;
}




LocationInfoMenu::LocationInfoMenu(RouteNetwork &network) : Menu(network) {}

void LocationInfoMenu::show() {
    std::cout << CLEAR;
    int options = 0;

    std::cout << "(" << ++options << ") >> " << "Get location info by ID" << std::endl;
    std::cout << "(" << ++options << ") >> " << "Get location info by code" << std::endl;
    std::cout << "(" << ++options << ") >> " << "Get location info by name" << std::endl;
    std::cout << "(0) >> Go back "<< std::endl;
}



Menu *LocationInfoMenu::getNextMenu() {
    int option;
    if(!InputHandler::get(option)) {
        return invalidInput();
    }

    std::string input;
    switch (option) {
        case 0: {
            return nullptr;
        }
        case 1: {
            std::cout << "Please introduce the location ID." << std::endl;
            input = InputHandler::getInput();
            route_network_.showLocationInfoById(input);
        } break;
        case 2: {
            std::cout << "Please introduce the location code." << std::endl;
            input = InputHandler::getInput();
            route_network_.showLocationInfoByCode(input);
        }   break;
        case 3: {
            std::cout << "Please introduce the location name." << std::endl;
            input = InputHandler::getInput();
            route_network_.showLocationInfoByName(input);
        } break;
    }

    InputHandler::waitForInput();

    return this;
}



RouteMenu::RouteMenu(RouteNetwork &network) : Menu(network) {}

void RouteMenu::show() {
    std::cout << CLEAR;
    int options = 0;

    std::cout << "(" << ++options << ") >> " << "Get unrestricted route from source to destination by ID" << std::endl;
    std::cout << "(" << ++options << ") >> " << "Get unrestricted route from source to destination by code  TO IMPLEMENT" << std::endl;
    std::cout << "(" << ++options << ") >> " << "Get unrestricted route from source to destination by name" << std::endl;
    std::cout << "(0) >> Go back "<< std::endl;
}



Menu *RouteMenu::getNextMenu() {
    int option;
    if(!InputHandler::get(option)) {
        return invalidInput();
    }

    std::string input;
    switch (option) {
        case 0: {
            return nullptr;
        }
        case 1: {
            std::cout << "Please introduce the source location ID." << std::endl;
            input = InputHandler::getInput();
            std::string source = input;

            std::cout << "Please introduce the destination location ID." << std::endl;
            input = InputHandler::getInput();
            std::string dest = input;

            route_network_.unrestrictedDrivingById(source, dest, route_network_);

        } break;
        case 2: {
            std::cout << "Please introduce the location code." << std::endl;
            input = InputHandler::getInput();
            route_network_.showLocationInfoByCode(input);
        }   break;
        case 3: {
            std::cout << "Please introduce the location name." << std::endl;
            input = InputHandler::getInput();
            route_network_.showLocationInfoByName(input);
        } break;
    }

    InputHandler::waitForInput();

    return this;
}
