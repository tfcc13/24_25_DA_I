#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H
#include <iostream>
#include <sstream>

#include "RequestProcessor.h"

class InputHandler {

    public:

    /**
     *@brief Ignores an input after pressing the key enter
     */
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

    /**
    * @brief Converts a string to lowercase.
    * @param input The input string.
    * @return A new string with all characters converted to lowercase.
    */
    static std::string toLowerString(const std::string& input);

    /**
    * @brief Converts a string to uppercase.
    * @param input The input string.
    * @return A new string with all characters converted to uppercase.
    */
    static std::string toUpperString(const std::string& input);

    /**
    * @brief Strips whitespaces, tab and new line chars.
    * @param input The input string.
    * @return A new trimmed string.
    */
    static void trimString(std::string &str);

    /**
     * @brief Reads from file and transforms into Request
     * @param filepath Path to file to read input
     * @return Request with given values in read file
     */
    static Request parseInputFile(const std::string& filepath, bool &correct);

    /**
    * @brief Parses driving-related request parameters from key-value pairs.
    * @param request The request object to be populated.
    * @param key The key representing the request parameter.
    * @param value The value associated with the key.
    * @param index The index of the parameter in the input data.
    * @param correct A reference to a boolean that indicates whether parsing was successful.
    */
    static void parseDriving(Request &request, std::string key, std::string value, int index, bool &correct);

    /**
    * @brief Parses driving-walking request parameters from key-value pairs.
    * @param request The request object to be populated.
    * @param key The key representing the request parameter.
    * @param value The value associated with the key.
    * @param index The index of the parameter in the input data.
    * @param correct A reference to a boolean that indicates whether parsing was successful.
    */
    static void parseDrivingWalking(Request &request, std::string key, std::string value, int index, bool &correct);

    /**
    * @brief Parses a comma-separated string of integers into a vector of integers.
    * @param input The input string containing integers separated by commas.
    * @return A vector containing the parsed integers. Returns an empty vector if the input is invalid.
    */
    static std::vector<int> parseIntSepByComma(std::string input) ;

    /**
    * @brief Parses a string representing a pair of integers separated by a hyphen ('-').
    * @param input The input string in the format "X-Y".
    * @return A pair of integers. Returns {-1, -1} if the input format is invalid.
    */
    static std::pair<int, int> parseIntPair(std::string input) ;

    /**
    * @brief Parses a comma-separated list of integer pairs.
    * @param input The input string containing pairs in "X-Y" format, separated by commas.
    * @return A vector of integer pairs. Returns an empty vector if any pair is invalid.
    */
    static std::vector<std::pair<int,int>> parseIntPairSepByComma(std::string input) ;

    /**
    * @brief Parses a comma-separated string of words.
    * @param input The input string containing words separated by commas.
    * @return A vector of strings representing the parsed words.
    */
    static std::vector<std::string> parseStrSepByComma(std::string input);

    /**
    * @brief Parses a string representing a pair of words separated by a hyphen ('-').
    * @param input The input string in the format "word1-word2".
    * @return A pair of strings. Returns {"", ""} if the input format is invalid.
    */
    static std::pair<std::string, std::string> parseStrPair(std::string input);

    /**
    * @brief Parses a comma-separated list of word pairs.
    * @param input The input string containing pairs in "word1-word2" format, separated by commas.
    * @return A vector of string pairs. Returns an empty vector if any pair is invalid.
    */
    static std::vector<std::pair<std::string,std::string>> parseStrPairSepByComma(std::string input);

    /**
    * @brief Converts an input string by replacing underscores ('_') with spaces.
    * @param input The input string to process.
    * @return A modified string with underscores replaced by spaces. Returns an empty string if input contains spaces.
    */
    static std::string parseName(std::string input);
};
#endif //INPUTHANDLER_H
