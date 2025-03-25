//
// Created by tiago on 26/02/2025.
//

#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H
#include <iostream>
#include <sstream>

#include "RequestProcessor.h"

class InputHandler {

    public:

    /// Ignores an input after pressing the key enter
    static void waitForInput();

    /**
     *@brief Waits for an user input.
     * @return Returns the user input.
     */
    static std::string getInput();

    /**
     * @brief Waits for an user input string.
     * @return the user input.
     */
    static std::string getInputLine();

    /**
     * @brief Converts an user input string to a variable T.
     * @tparam T Type of the expected conversion defined by the input string.
     * @param str String read in the input.
     * @param var
     * @return  true if conversion is successful.
     */
    template <typename T>
    static bool convertStrToVar(const std::string &str, T &var){
        std::istringstream ss{str};
        ss >> var;
        if(ss.fail() || !ss.eof()){
            return false;
        }
        return true;
    }

    /**
     * @brief Expects a variable of type T from input cin.
     * @tparam T Type of the expected variable.
     * @param var Variable used to extract the expected type.
     * @return true if it gets the expected variable.
     */
    template<typename T>
    static bool get(T &var) {
        std::string str = getInput();
        if (str.empty()) {
            return false;
        }
        return convertStrToVar(str, var);
    }

    static std::string toLowerString(const std::string& input);
    static std::string toUpperString(const std::string& input);



    /**
     * @brief Reads from file and transforms into Request
     * @param filepath Path to file to read input
     * @return Request with given values in read file
     */
    static Request parseInputFile(const std::string& filepath, bool &correct);

    static void parseDriving(Request &request, std::string key, std::string value, int index, bool &correct);
    static void parseDrivingWalking(Request &request, std::string key, std::string value, int index, bool &correct);





};
#endif //INPUTHANDLER_H
