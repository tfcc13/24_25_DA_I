#ifndef ROUTE_H
#define ROUTE_H
#include <Graph.h>
#include <climits>


/**
* @class Route
* @brief Represents a route between two locations, extending the Edge class.
*
* This class models a route with attributes for walking and driving times, as well as
* the origin and destination locations.
*/
class Route : public Edge<std::string> {
private:
    double walking_time_; /**< Time required to traverse the route by walking. */
    double driving_time_; /**< Time required to traverse the route by driving. */
    std::string orig_; /**< Origin location identifier. */
    std::string dest_; /**< Destination location identifier. */

public:
    /**
    * @brief Constructs a Route object.
    * @param orig Pointer to the origin vertex.
    * @param dest Pointer to the destination vertex.
    * @param walking_time The time needed to walk along this route.
    * @param driving_time The time needed to drive along this route.
    */
    Route(Vertex<std::string>* orig, Vertex<std::string>* dest, double walking_time, double driving_time);

    /**
    * @brief Gets the walking time for this route.
    * @return Walking time as a double.
    */
    double get_walking_time();

    /**
    * @brief Gets the driving time for this route.
    * @return Driving time as a double.
    */
    double get_driving_time();

    /**
    * @brief Gets the origin location identifier.
    * @return Origin location as a string.
    */
    std::string get_orig();

    /**
    * @brief Gets the destination location identifier.
    * @return Destination location as a string.
    */
    std::string get_dest();

};

#endif //ROUTE_H
