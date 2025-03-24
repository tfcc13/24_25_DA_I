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
    std::cout << "(0) >> Exit "<< std::endl;
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
            // to implement
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
    std::cout << "(0) >> Exit "<< std::endl;
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

        if (files.empty()) {
            std::cout << "No files found in input directory\n"
            << "Insert input files in the input folder\n";
            return;
        }

        std::sort(files.begin(), files.end());

        for (const auto &file : files) {
            std::cout << "(" << ++options << ") >> " << file.string() << std::endl;
        }
    } catch (const std::filesystem::filesystem_error &e) {
        std::cout << e.code() << " " << e.what() << std::endl;
    }


    std::cout << "(0) >> Exit "<< std::endl;
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
    bool correctFile = true;
    Request r = InputHandler::parseInputFile(filepath, correctFile);

    if (correctFile) {
        RequestProcessor::processRequest(r, route_network_);
    }
    else std::cout << "Input file not within predefined standards" << std::endl;

    InputHandler::waitForInput();

    return this;
}



