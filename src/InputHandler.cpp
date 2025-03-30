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

// change this
void InputHandler::trimString(std::string &str) {
    while (!str.empty() && (str.back() == '\r' || str.back() == ','))
        str.pop_back();
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

void InputHandler::parseDriving(Request &route, std::string key, std::string value, int idx, bool &correct) {
    switch (idx) {
        case 0: {
            if (key != "Mode") { correct = false; break;}
            route.mode = value;
            break;
        }
        case 1: {
            if (key != "Source") {correct = false; break;}
            try {route.src = std::stoi(value);}
            catch (const std::invalid_argument&) {correct = false;}
            break;
        }
        case 2: {
            if (key != "Destination") {correct = false; break;}
            try {route.dest = std::stoi(value);}
            catch (const std::invalid_argument&) {correct = false;}
            break;
        }
        case 3: {
            if (key != "AvoidNodes") {correct = false; break;}
            std::istringstream ss(value);
            int node;
            while (ss >> node) {
                route.avoidNodes.push_back(node);
                if (ss.peek() == ',') ss.ignore();
            }
            break;
        }
        case 4: {
            if (key != "AvoidSegments") correct = false;
            else {
                std::istringstream ss(value);
                char token;
                int a, b;

                while (ss >> token) {
                    if (token != '(') continue;
                    ss >> a >> token >> b >> token;
                    route.avoidSegments.emplace_back(a,b);
                }
            }
             break;
        }
        case 5: {
            if (key != "IncludeNode") {correct = false; break;}
             if(!value.empty()) {
                 try {route.includeNode = std::stoi(value);}
                 catch (const std::invalid_argument&) {correct = false;}
             }
            break;
        }
        default:break;
    }
}




void InputHandler::parseDrivingWalking(Request &route, std::string key, std::string value, int idx, bool &correct) {
    switch (idx) {
        case 0: {
            if (key != "Mode") { correct = false; break;}
            route.mode = value;
            break;
        }
        case 1: {
            if (key != "Source") {correct = false; break;}
            try {route.src = std::stoi(value);}
            catch (const std::invalid_argument&) {correct = false;}
            break;
        }
        case 2: {
            if (key != "Destination") {correct = false; break;}
            try {route.dest = std::stoi(value);}
            catch (const std::invalid_argument&) {correct = false;}
            break;
        }
        case 3: {
            if (key != "MaxWalkTime") {correct = false; break;}
            try {route.maxWalkTime = std::stoi(value);}
            catch (const std::invalid_argument&) {correct = false;}
            break;
        }
        case 4: {
            if (key != "AvoidNodes") correct = false;
            else {
                std::istringstream ss(value);
                int node;
                while (ss >> node) {
                    route.avoidNodes.push_back(node);
                    if (ss.peek() == ',') ss.ignore();
                }
            }
            break;
        }
        case 5: {
            if (key != "AvoidSegments") correct = false;
            else {
                std::istringstream ss(value);
                char token;
                int a, b;

                while (ss >> token) {
                    if (token != '(') continue;

                    ss >> a >> token >> b >> token;

                    route.avoidSegments.emplace_back(a,b);
                }
            }
            break;
        }
        default: break;
    }
}


Request InputHandler::parseInputFile(const std::string& filepath, bool &correct) {
    Request route;
    std::ifstream file(filepath);
    std::string line;

    if (!file) {
        std::cerr << "Error: Could not open file " << filepath << "\n";
        correct = false;
        return route;
    }

    int idx = 0;

    while(getline(file, line)) {
        std::istringstream iss(line);
        std::string key, value;
        getline(iss >> std::ws, key, ':'); // Remove leading whitespace
        getline(iss >> std::ws, value);    // Remove leading whitespace in value
        value = value.substr(0, value.find("\r"));

        if (value == "driving" || route.mode == "driving") {
            parseDriving(route, key, value, idx, correct);
        }
        else parseDrivingWalking(route, key, value, idx, correct);
        idx++;

    }

    if (route.mode == "driving" && (idx >= 4 && idx <= 5)) correct = false;
    if (route.mode == "driving-walking" && idx != 6) correct = false;

    file.close();
    return route;
}

 std::vector<int> InputHandler::parseIntSepByComma(std::string input) {
    std::vector<int> avoidLocations;
    if (input.find(' ') != std::string::npos) {
        std::cerr << "Invalid input: spaces are not allowed. Please use '1,2,3' format.\n";
        return avoidLocations;
    }

    std::istringstream ss(input);
    std::string token;

    while (std::getline(ss, token, ',')) {
        try {
            avoidLocations.push_back(std::stoi(token)); // Convert to int
        } catch (const std::invalid_argument&) {
            std::cerr << "Invalid number detected: '" << token << "'. Skipping.\n";
            avoidLocations.clear();
            return avoidLocations;
        }
    }

    return avoidLocations;
}

std::pair<int, int> InputHandler::parseIntPair(std::string input) {
    size_t pos = input.find('-');
    if (pos == std::string::npos) {
        std::cerr << "Invalid input: Expected format 'X-Y' (e.g., '1-2').\n";
        return {-1, -1};
    }

    try {
        int first = std::stoi(input.substr(0, pos));
        int second = std::stoi(input.substr(pos + 1));
        return {first, second};
    } catch (const std::invalid_argument&) {
        std::cerr << "Invalid input: Non-numeric values found.\n";
    } catch (const std::out_of_range&) {
        std::cerr << "Invalid input: Number out of range.\n";
    }

    return {-1, -1};

}

std::vector<std::pair<int,int>> InputHandler::parseIntPairSepByComma(std::string input) {
    std::vector<std::pair<int, int>> pairs;

    if (input.find(' ') != std::string::npos) {
        std::cerr << "Invalid input: spaces are not allowed. Please use '1_2,3_4,2_5' format.\n";
        return pairs;
    }


    std::stringstream ss(input);
    std::string pairStr;

    while (std::getline(ss, pairStr, ',')) {
        auto parsedPair = parseIntPair(pairStr);
        if (parsedPair.first == parsedPair.second && parsedPair.first == -1) {
            pairs.clear();
            return pairs;
        }
        pairs.push_back(parsedPair);
    }

    return pairs;

}

std::vector<std::string> InputHandler::parseStrSepByComma(std::string input) {
    std::vector<std::string> avoidLocations;
    if (input.find(' ') != std::string::npos) {
        std::cerr << "Invalid input: spaces are not allowed. Please use '1,2,3' format.\n";
        return avoidLocations;
    }

    std::string temp = parseName(input);
    std::stringstream ss(temp);

    std::string token;

    while (std::getline(ss, token, ',')) {
        avoidLocations.push_back(token);
    }

    return avoidLocations;
}

std::pair<std::string, std::string> InputHandler::parseStrPair(std::string input) {
    size_t pos = input.find('-');
    if (pos == std::string::npos) {
        std::cerr << "Invalid input: Expected format 'X-Y' (e.g., '1-2').\n";
        return {"", ""};
    }

    std::string first = input.substr(0, pos);
    std::string second = input.substr(pos + 1);
        return {first, second};
}

std::vector<std::pair<std::string,std::string>> InputHandler::parseStrPairSepByComma(std::string input) {
    std::vector<std::pair<std::string, std::string>> pairs;

    if (input.find(' ') != std::string::npos) {
        std::cerr << "Invalid input: spaces are not allowed. Please use 'ALI-ESTAS,LOL-REI,MALA_CIAO' format.\n";
        return pairs;
    }

    std::string temp = parseName(input);

    std::stringstream ss(temp);
    std::string pairStr;

    while (std::getline(ss, pairStr, ',')) {
        auto parsedPair = parseStrPair(pairStr);
        if (parsedPair.first == parsedPair.second && parsedPair.first.empty()) {
            pairs.clear();
            return pairs;
        }
        pairs.push_back(parsedPair);
    }

    return pairs;

}

std::string InputHandler::parseName(std::string input) {
    if (input.find(' ') != std::string::npos) {
        std::cerr << "Invalid input: spaces are not allowed. Please use 'ALI-ESTAS,LOL-REI,MALA_CIAO' format.\n";
        return "";
    }

    std::string output = input;
    for (char &c : output) {
        if (c == '_') {
            c = ' ';  // Replace underscore with space
        }
    }
    return output;

}


