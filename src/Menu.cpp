#include "Menu.h"
#include <filesystem>

#include "RequestProcessor.h"

Menu::Menu(RouteNetwork &route_network) : route_network_(route_network) {}

Menu *Menu::invalidInput() {
    std::cout << "Invalid input. Please try again." << std::endl;
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
            return  new MainRouteMenu(route_network_);
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
                std::cout << location.second->getId() << " " << location.second->getCode() << " " << location.second->getName() << " " << location.second->getAdj().size() <<  std::endl;
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
    bool correctFile = true;
    Request r = InputHandler::parseInputFile(filepath, correctFile);

    if (correctFile) {
        RequestProcessor::processRequest(r, route_network_, ID_MODE);
    }
    else std::cout << "Input file not within predefined standards" << std::endl;

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
            input = InputHandler::parseName(input);
            route_network_.showLocationInfoByName(input);
        } break;
    }

    InputHandler::waitForInput();

    return this;
}



MainRouteMenu::MainRouteMenu(RouteNetwork &network) : Menu(network) {}

void MainRouteMenu::show() {
    std::cout << CLEAR;
    int options = 0;

    std::cout << "(" << ++options << ") >> " << "Get routes by ID" << std::endl;
    std::cout << "(" << ++options << ") >> " << "Get routes by code" << std::endl;
    std::cout << "(" << ++options << ") >> " << "Get routes by name" << std::endl;
    std::cout << "(0) >> Go back "<< std::endl;
}

Menu *MainRouteMenu::getNextMenu() {
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
            return new IdRouteMenu(route_network_);
        }
        case 2: {
            return new CodeRouteMenu(route_network_);
        }
        case 3: {
            return new NameRouteMenu(route_network_);
        }
    }

    InputHandler::waitForInput();

    return this;
}



IdRouteMenu::IdRouteMenu(RouteNetwork &network) : Menu(network) {}

void IdRouteMenu::show() {
    std::cout << CLEAR;
    int options = 0;

    std::cout << "(" << ++options << ") >> " << "Get unrestricted driving route from source to destination" << std::endl;
    std::cout << "(" << ++options << ") >> " << "Get environmentally friendly route from source to destination" << std::endl;
    std::cout << "(" << ++options << ") >> " << "Get restricted driving route from source to destination" << std::endl;
    std::cout << "(" << ++options << ") >> " << "Get restricted  environmentally friendly route from source to destination" << std::endl;
    std::cout << "(0) >> Go back "<< std::endl;
}


Menu *IdRouteMenu::getNextMenu() {
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
            int src;
            if (!InputHandler::get(src)) {
                return invalidInput();
            }
            std::cout << "Please introduce the destination location ID." << std::endl;

            int dest;
            if (!InputHandler::get(dest)) {
                return invalidInput();
            }

            route_network_.routeById(src, dest, route_network_, ID_MODE, DRIVING_MODE);

        } break;

        case 2: {
            std::cout << "Please introduce the source location ID." << std::endl;
            int src;
            if (!InputHandler::get(src)) {
                return invalidInput();
            }
            std::cout << "Please introduce the destination location ID." << std::endl;

            int dest;
            if (!InputHandler::get(dest)) {
                return invalidInput();
            }

            std::cout << "Please introduce maximum walking time." << std::endl;
            int max_time;
            if (!InputHandler::get(max_time)) {
                return invalidInput();
            }

            route_network_.routeById(src, dest, route_network_, ID_MODE, WALKING_MODE,max_time);

        } break;

        case 3: {
            std::cout << "Please introduce the source location id." << std::endl;
            int source;

            if (!InputHandler::get(source)) {
                return invalidInput();
            }


            std::cout << "Please introduce the destination location id." << std::endl;
            int dest;

            if (!InputHandler::get(dest)) {
                return invalidInput();
            }

            std::vector<int> avoidNodes;
            std::vector<std::pair<int,int>> avoidRoute;
            int includeNode = -1;
            std::cout << "Do you wish to avoid any location ? (Write 'y' or'yes' if you do) ." << std::endl;

            input = InputHandler::getInput();

            if (InputHandler::toLowerString(input) == "yes" || InputHandler::toLowerString(input) == "y") {
                std::cout << "Please introduce the locations Id to avoid separated by commas and without spaces (i.e. '1,2,3')." << std::endl;
                input = InputHandler::getInput();
                avoidNodes = InputHandler::parseIntSepByComma(input);
                if (avoidNodes.empty()) {
                    return invalidInput();
                }
            }

            std::cout << "Do you wish to avoid any route ? (Write 'y' or'yes' if you do) ." << std::endl;

            input = InputHandler::getInput();

            if (InputHandler::toLowerString(input) == "yes" || InputHandler::toLowerString(input) == "y") {
                std::cout <<  "Please enter the source and destination IDs to avoid, separated by an hifen ('-') and listed without spaces, using commas between pairs (e.g., '1-2,3-4,3-5')." << std::endl;
                input = InputHandler::getInput();
                avoidRoute = InputHandler::parseIntPairSepByComma(input);
                if (avoidRoute.empty()) {
                    return invalidInput();
                }
            }

            std::cout << "Do you wish to include a stop location ? (Write 'y' or'yes' if you do) ." << std::endl;

            input = InputHandler::getInput();

            if (InputHandler::toLowerString(input) == "yes" || InputHandler::toLowerString(input) == "y") {
                std::cout <<  "Please enter the location ID to include" << std::endl;
                if (!InputHandler::get(includeNode)) {
                    return invalidInput();
                }
            }

            route_network_.restrictedRouteById(source, dest, route_network_, ID_MODE, DRIVING_MODE,avoidNodes, avoidRoute,includeNode);


        } break;

        case 4: {
            std::cout << "Please introduce the source location id." << std::endl;
            int source;

            if (!InputHandler::get(source)) {
                return invalidInput();
            }


            std::cout << "Please introduce the destination location id." << std::endl;
            int dest;

            if (!InputHandler::get(dest)) {
                return invalidInput();
            }

            std::vector<int> avoidNodes;
            std::vector<std::pair<int,int>> avoidRoute;
            int includeNode = -1;
            std::cout << "Do you wish to avoid any location ? (Write 'y' or'yes' if you do) ." << std::endl;

            input = InputHandler::getInput();

            if (InputHandler::toLowerString(input) == "yes" || InputHandler::toLowerString(input) == "y") {
                std::cout << "Please introduce the locations Id to avoid separated by commas and without spaces (i.e. '1,2,3')." << std::endl;
                input = InputHandler::getInput();
                avoidNodes = InputHandler::parseIntSepByComma(input);
                if (avoidNodes.empty()) {
                    return invalidInput();
                }
            }

            std::cout << "Do you wish to avoid any route ? (Write 'y' or'yes' if you do) ." << std::endl;

            input = InputHandler::getInput();

            if (InputHandler::toLowerString(input) == "yes" || InputHandler::toLowerString(input) == "y") {
                std::cout <<  "Please enter the source and destination IDs to avoid, separated by an hifen ('-') and listed without spaces, using commas between pairs (e.g., '1-2,3-4,3-5')." << std::endl;
                input = InputHandler::getInput();
                avoidRoute = InputHandler::parseIntPairSepByComma(input);
                if (avoidRoute.empty()) {
                    return invalidInput();
                }
            }

            std::cout << "Please introduce maximum walking time." << std::endl;

            int max_time;
            if (!InputHandler::get(max_time)) {
                return invalidInput();
            }


            route_network_.restrictedRouteById(source, dest, route_network_, ID_MODE, WALKING_MODE,avoidNodes, avoidRoute,includeNode,max_time);


        } break;

    }

    InputHandler::waitForInput();

    return this;
}


CodeRouteMenu::CodeRouteMenu(RouteNetwork &network) : Menu(network) {}

void CodeRouteMenu::show() {
    std::cout << CLEAR;
    int options = 0;

    std::cout << "(" << ++options << ") >> " << "Get unrestricted driving route from source to destination" << std::endl;
    std::cout << "(" << ++options << ") >> " << "Get environmentally friendly route from source to destination" << std::endl;
    std::cout << "(" << ++options << ") >> " << "Get restricted driving route from source to destination" << std::endl;
    std::cout << "(" << ++options << ") >> " << "Get restricted  environmentally friendly route from source to destination" << std::endl;
    std::cout << "(0) >> Go back "<< std::endl;
}


Menu *CodeRouteMenu::getNextMenu() {
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
            std::cout << "Please introduce the source location code." << std::endl;
            input = InputHandler::getInput();
            std::string source = input;


            std::cout << "Please introduce the destination location code." << std::endl;
            input = InputHandler::getInput();
            std::string dest = input;

            route_network_.routeByCode(source, dest, route_network_, DRIVING_MODE);

        }   break;

        case 2: {
            std::cout << "Please introduce the source location code." << std::endl;
            input = InputHandler::getInput();
            std::string source = input;


            std::cout << "Please introduce the destination location code." << std::endl;
            input = InputHandler::getInput();
            std::string dest = input;

            std::cout << "Please introduce maximum walking time." << std::endl;
            int max_time;
            if (!InputHandler::get(max_time)) {
                return invalidInput();
            }

            route_network_.routeByCode(source, dest, route_network_, WALKING_MODE, max_time);

        }   break;

        case 3: {
            std::cout << "Please introduce the source location code." << std::endl;
            std::string source = InputHandler::getInput();



            std::cout << "Please introduce the destination location code." << std::endl;
            std::string dest = InputHandler::getInput();


            std::vector<std::string> avoidNodes;
            std::vector<std::pair<std::string,std::string>> avoidRoute;
            std::string includeNode = "";
            std::cout << "Do you wish to avoid any location ? (Write 'y' or'yes' if you do) ." << std::endl;

            input = InputHandler::getInput();

            if (InputHandler::toLowerString(input) == "yes" || InputHandler::toLowerString(input) == "y") {
                std::cout << "Please introduce the locations code to avoid separated by commas and without spaces (i.e. '1,2,3')." << std::endl;
                input = InputHandler::getInput();
                avoidNodes = InputHandler::parseStrSepByComma(input);
                if (avoidNodes.empty()) {
                    return invalidInput();
                }
            }

            std::cout << "Do you wish to avoid any route ? (Write 'y' or'yes' if you do) ." << std::endl;

            input = InputHandler::getInput();

            if (InputHandler::toLowerString(input) == "yes" || InputHandler::toLowerString(input) == "y") {
                std::cout <<  "Please enter the source and destination codes to avoid, separated by an hifen ('-') and listed without spaces, using commas between pairs (e.g., '1-2,3-4,3-5')." << std::endl;
                input = InputHandler::getInput();
                avoidRoute = InputHandler::parseStrPairSepByComma(input);
                if (avoidRoute.empty()) {
                    return invalidInput();
                }
            }

            std::cout << "Do you wish to include a stop location ? (Write 'y' or'yes' if you do) ." << std::endl;

            input = InputHandler::getInput();

            if (InputHandler::toLowerString(input) == "yes" || InputHandler::toLowerString(input) == "y") {
                std::cout <<  "Please enter the location code to include" << std::endl;
                if (!InputHandler::get(includeNode)) {
                    return invalidInput();
                }
            }

            route_network_.restrictedRouteByCode(source, dest, route_network_, DRIVING_MODE,avoidNodes, avoidRoute,includeNode);

        } break;

         case 4: {
            std::cout << "Please introduce the source location code." << std::endl;
            std::string source = InputHandler::getInput();



            std::cout << "Please introduce the destination location code." << std::endl;
            std::string dest = InputHandler::getInput();


            std::vector<std::string> avoidNodes;
            std::vector<std::pair<std::string,std::string>> avoidRoute;
            std::string includeNode = "";
            std::cout << "Do you wish to avoid any location ? (Write 'y' or'yes' if you do) ." << std::endl;

            input = InputHandler::getInput();

            if (InputHandler::toLowerString(input) == "yes" || InputHandler::toLowerString(input) == "y") {
                std::cout << "Please introduce the locations code to avoid separated by commas and without spaces (i.e. '1,2,3')." << std::endl;
                input = InputHandler::getInput();
                avoidNodes = InputHandler::parseStrSepByComma(input);
                if (avoidNodes.empty()) {
                    return invalidInput();
                }
            }

            std::cout << "Do you wish to avoid any route ? (Write 'y' or'yes' if you do) ." << std::endl;

            input = InputHandler::getInput();

            if (InputHandler::toLowerString(input) == "yes" || InputHandler::toLowerString(input) == "y") {
                std::cout <<  "Please enter the source and destination codes to avoid, separated by an hifen ('-') and listed without spaces, using commas between pairs (e.g., '1-2,3-4,3-5')." << std::endl;
                input = InputHandler::getInput();
                avoidRoute = InputHandler::parseStrPairSepByComma(input);
                if (avoidRoute.empty()) {
                    return invalidInput();
                }
            }

             std::cout << "Please introduce maximum walking time." << std::endl;

             int max_time;
             if (!InputHandler::get(max_time)) {
                 return invalidInput();
             }

            route_network_.restrictedRouteByCode(source, dest, route_network_, WALKING_MODE,avoidNodes, avoidRoute,includeNode,max_time);

        } break;
    }

    InputHandler::waitForInput();

    return this;
}


NameRouteMenu::NameRouteMenu(RouteNetwork &network) : Menu(network) {}

void NameRouteMenu::show() {
    std::cout << CLEAR;
    int options = 0;

    std::cout << "(" << ++options << ") >> " << "Get unrestricted driving route from source to destination" << std::endl;
    std::cout << "(" << ++options << ") >> " << "Get environmentally friendly route from source to destination" << std::endl;
    std::cout << "(" << ++options << ") >> " << "Get restricted driving route from source to destination" << std::endl;
    std::cout << "(" << ++options << ") >> " << "Get restricted  environmentally friendly route from source to destination" << std::endl;
    std::cout << "(0) >> Go back "<< std::endl;
}


Menu *NameRouteMenu::getNextMenu() {
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
            std::cout << "Please introduce the source location name. Use '_' instead of white spaces(e.g. 'Tv._Contumil')" << std::endl;
            input = InputHandler::getInput();
            std::string source = InputHandler::parseName(input);


            std::cout << "Please introduce the destination location name. Use '_' instead of white spaces(e.g. 'Tv._Contumil')" << std::endl;
            input = InputHandler::getInput();
            std::string dest = InputHandler::parseName(input);

            route_network_.routeByName(source, dest, route_network_, DRIVING_MODE);

        } break;


        case 2: {
            std::cout << "Please introduce the source location name. Use '_' instead of white spaces(e.g. 'Tv._Contumil')" << std::endl;
            input = InputHandler::getInput();
            std::string source = InputHandler::parseName(input);


            std::cout << "Please introduce the destination location name. Use '_' instead of white spaces(e.g. 'Tv._Contumil')" << std::endl;
            input = InputHandler::getInput();
            std::string dest = InputHandler::parseName(input);

            std::cout << "Please introduce maximum walking time." << std::endl;

            int max_time;

            if(!InputHandler::get(max_time)) {
                return invalidInput();
            }

            route_network_.routeByName(source, dest, route_network_, WALKING_MODE, max_time);

        } break;

        case 3: {
            std::cout << "Please introduce the source location name. Use '_' instead of white spaces(e.g. 'Tv._Contumil')" << std::endl;
            input = InputHandler::getInput();
            std::string source = InputHandler::parseName(input);


            std::cout << "Please introduce the destination location name. Use '_' instead of white spaces(e.g. 'Tv._Contumil')" << std::endl;
            input = InputHandler::getInput();
            std::string dest = InputHandler::parseName(input);


            std::vector<std::string> avoidNodes;
            std::vector<std::pair<std::string,std::string>> avoidRoute;
            std::string includeNode = "";
            std::cout << "Do you wish to avoid any location ? (Write 'y' or'yes' if you do) ." << std::endl;

            input = InputHandler::getInput();

            if (InputHandler::toLowerString(input) == "yes" || InputHandler::toLowerString(input) == "y") {
                std::cout << "Please introduce the locations code to avoid separated by commas and without spaces (i.e. '1,2,3')." << std::endl;
                input = InputHandler::getInput();
                avoidNodes = InputHandler::parseStrSepByComma(input);
                if (avoidNodes.empty()) {
                    return invalidInput();
                }
            }

            std::cout << "Do you wish to avoid any route ? (Write 'y' or'yes' if you do) ." << std::endl;

            input = InputHandler::getInput();

            if (InputHandler::toLowerString(input) == "yes" || InputHandler::toLowerString(input) == "y") {
                std::cout <<  "Please enter the source and destination names to avoid, separated by an hifen ('-') and listed without spaces, using commas between pairs (e.g., '1-2,3-4,3-5')." << std::endl;
                input = InputHandler::getInput();
                avoidRoute = InputHandler::parseStrPairSepByComma(input);
                if (avoidRoute.empty()) {
                    return invalidInput();
                }
            }

            std::cout << "Do you wish to include a stop location ? (Write 'y' or'yes' if you do) ." << std::endl;

            input = InputHandler::getInput();

            if (InputHandler::toLowerString(input) == "yes" || InputHandler::toLowerString(input) == "y") {
                std::cout <<  "Please enter the location name to include" << std::endl;
                if (!InputHandler::get(includeNode)) {
                    return invalidInput();
                }
                includeNode = InputHandler::parseName(includeNode);
            }

            route_network_.restrictedRouteByName(source, dest, route_network_, DRIVING_MODE,avoidNodes, avoidRoute,includeNode);

        } break;

         case 4: {
             std::cout << "Please introduce the source location name. Use '_' instead of white spaces(e.g. 'Tv._Contumil')" << std::endl;
             input = InputHandler::getInput();
             std::string source = InputHandler::parseName(input);


             std::cout << "Please introduce the destination location name. Use '_' instead of white spaces(e.g. 'Tv._Contumil')" << std::endl;
             input = InputHandler::getInput();
             std::string dest = InputHandler::parseName(input);


            std::vector<std::string> avoidNodes;
            std::vector<std::pair<std::string,std::string>> avoidRoute;
            std::string includeNode = "";
            std::cout << "Do you wish to avoid any location ? (Write 'y' or'yes' if you do) ." << std::endl;

            input = InputHandler::getInput();

            if (InputHandler::toLowerString(input) == "yes" || InputHandler::toLowerString(input) == "y") {
                std::cout << "Please introduce the locations code to avoid separated by commas and without spaces (i.e. '1,2,3')." << std::endl;
                input = InputHandler::getInput();
                avoidNodes = InputHandler::parseStrSepByComma(input);
                if (avoidNodes.empty()) {
                    return invalidInput();
                }
            }

            std::cout << "Do you wish to avoid any route ? (Write 'y' or'yes' if you do) ." << std::endl;

            input = InputHandler::getInput();

            if (InputHandler::toLowerString(input) == "yes" || InputHandler::toLowerString(input) == "y") {
                std::cout <<  "Please enter the source and destination codes to avoid, separated by an hifen ('-') and listed without spaces, using commas between pairs (e.g., '1-2,3-4,3-5')." << std::endl;
                input = InputHandler::getInput();
                avoidRoute = InputHandler::parseStrPairSepByComma(input);
                if (avoidRoute.empty()) {
                    return invalidInput();
                }
            }

             std::cout << "Please introduce maximum walking time." << std::endl;

             int max_time;
             if (!InputHandler::get(max_time)) {
                 return invalidInput();
             }

            route_network_.restrictedRouteByName(source, dest, route_network_, WALKING_MODE,avoidNodes, avoidRoute,includeNode,max_time);

        } break;

    }

    InputHandler::waitForInput();

    return this;
}
