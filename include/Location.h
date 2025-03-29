//
// Created by tiago on 26/02/2025.
//

#ifndef LOCATION_H
#define LOCATION_H

#include <Graph.h>
#include <string>

/**
 * @class Location
 * @brief Represents a location in the graph, inheriting from Vertex.
 */

class Location : public Vertex<std::string>{
private:
    std::string name_;
    std::string id_;
    std::string code_;
    bool canPark_;
public:

    /**
    * @brief Constructs a Location object.
    * @param name The name of the location.
    * @param id The unique identifier of the location.
    * @param code A specific code for the location.
    * @param canPark Boolean flag indicating whether parking is available.
    */
    Location(std::string name, std::string id, std::string code, bool canPark);

    /**
    * @brief Retrieves the name of the location.
    * @return The location name.
    */
    std::string getName();

    /**
    * @brief Retrieves the unique identifier of the location.
    * @return The location ID.
    */
    std::string getId();

    /**
    * @brief Retrieves the code of the location.
    * @return The location code.
    */
    std::string getCode();

    /**
    * @brief Checks if parking is available at the location.
    * @return True if parking is available, false otherwise.
    */
    bool getCanPark();

    /**
    * @brief Sets the name of the location.
    * @param name The new name for the location.
    */
    void setName(std::string name);

    /**
    * @brief Sets the code for the location.
    * @param code The new code for the location.
    */
    void setCode(std::string code);

    /**
    * @brief Sets whether parking is available at the location.
    * @param canPark Boolean flag indicating if parking is allowed.
    */
    void setCanPark(bool canPark);

    /**
    * @brief Prints the location's information.
    */
    void printInfo();
};


#endif //LOCATION_H
