#ifndef MENU_H
#define MENU_H

#define CLEAR "\033[2J\033[1;1H"

#include "RouteNetwork.h"
#include "InputHandler.h"
class RouteNetwork;

/**
* @brief Base class for all menu screens in the application.
*/
class Menu {
protected:
    RouteNetwork& route_network_; ///< Reference to the route network being managed.

    /**
    * @brief Displays an error message for invalid input and returns the current menu.
    * @return A pointer to the current menu.
    */
    Menu * invalidInput();

public:
    /**
    * @brief Constructor of a Menu object.
    * @param route_network Reference to the route network.
    */
    explicit Menu(RouteNetwork &route_network);

    /**
    * @brief Virtual destructor for proper polymorphic behavior.
    */
    virtual ~Menu() = default;

    /**
    * @brief Displays the menu interface.
    */
    virtual void show() = 0;

    /**
    * @brief Determines the next menu or action based on user input.
    * @return A pointer to the next menu.
    */
    virtual Menu *getNextMenu() = 0;
};

/**
* @brief Menu for handling dataset options.
*/
class DataMenu : public Menu {
public:
    /**
    * @brief Constructor of DataMenu object.
    * @param route_network Reference to the route network.
    */
    DataMenu(RouteNetwork &route_network);

    /**
    * @brief Displays the data menu interface.
    */
    void show() override;

    /**
    * @brief Determines the next menu based on user input.
    * @return A pointer to the next menu.
    */
    Menu *getNextMenu() override;
};

/**
* @brief Main menu of the application.
*/
class MainMenu : public Menu {
public:
    /**
    * @brief Constructor of MainMenu object.
    * @param route_network Reference to the route network.
    */
    MainMenu(RouteNetwork &route_network);

    /**
    * @brief Displays the main menu interface.
    */
    void show() override;

    /**
    * @brief Determines the next menu based on user input.
    * @return A pointer to the next menu.
    */
    Menu *getNextMenu() override;
};

/**
* @brief Menu for network and locations info.
*/
class LocationMenu : public Menu {
public:
    /**
    * @brief Constructs a LocationMenu object.
    * @param route_network Reference to the route network.
    */
    LocationMenu(RouteNetwork &route_network);

    /**
    * @brief Displays the location menu interface.
    */
    void show() override;

    /**
    * @brief Determines the next menu or action based on user input.
    * @return A pointer to the next menu.
    */
    Menu *getNextMenu() override;

};

/**
* @brief Menu for handling batch mode.
*/
class InputMenu : public Menu {
    public:
    /**
    * @brief Constructor of InputMenu object.
    * @param route_network Reference to the route network.
    */
    InputMenu(RouteNetwork &route_network);

    /**
    * @brief Displays the input menu interface.
    */
    void show() override;

    /**
    * @brief Determines the next menu or action based on user input.
    * @return A pointer to the next menu.
    */
    Menu *getNextMenu() override;
};


/**
* @brief Menu for displaying location information.
*/
class LocationInfoMenu : public Menu {
public:
    /**
    * @brief Constructor of LocationInfoMenu object.
    * @param route_network Reference to the route network.
    */
    LocationInfoMenu(RouteNetwork &route_network);

    /**
    * @brief Displays the location info menu interface.
    */
    void show() override;

    /**
    * @brief Determines the next menu or action based on user input.
    * @return A pointer to the next menu.
    */
    Menu *getNextMenu() override;
};

/**
* @brief Main menu for route search operations.
* Displays the search options, by ID, code and name.
*/
class MainRouteMenu : public Menu {
public:
    /**
    * @brief Constructs a MainRouteMenu object.
    * @param route_network Reference to the route network.
    */
    MainRouteMenu(RouteNetwork &route_network);

    /**
    * @brief Displays the main route menu interface.
    */
    void show() override;

    /**
    * @brief Determines the next menu based on user input.
    * @return A pointer to the next menu.
    */
    Menu *getNextMenu() override;
};

/**
* @brief Menu for searching routes by ID.
*/
class IdRouteMenu : public Menu {
public:
    /**
    * @brief Constructor of IdRouteMenu object.
    * @param route_network Reference to the route network.
    */
    IdRouteMenu(RouteNetwork &route_network);

    /**
    * @brief Displays the ID route menu interface.
    */
    void show() override;

    /**
    * @brief Determines the next menu or action based on user input.
    * @return A pointer to the next menu.
    */
    Menu *getNextMenu() override;
};

/**
* @brief Menu for searching routes by code.
*/
class CodeRouteMenu : public Menu {
public:
    /**
    * @brief Constructor of CodeRouteMenu object.
    * @param route_network Reference to the route network.
    */
    CodeRouteMenu(RouteNetwork &route_network);

    /**
    * @brief Displays the code route menu interface.
    */
    void show() override;

    /**
    * @brief Determines the next menu based on user input.
    * @return A pointer to the next menu.
    */
    Menu *getNextMenu() override;
};

/**
* @brief Menu for searching routes by name.
*/
class NameRouteMenu : public Menu {
public:
    /**
    * @brief Constructor of NameRouteMenu object.
    * @param route_network Reference to the route network.
    */
    NameRouteMenu(RouteNetwork &route_network);

    /**
    * @brief Displays the name route menu interface.
    */
    void show() override;

    /**
    * @brief Determines the next menu or action based on user input.
    * @return A pointer to the next menu.
    */
    Menu *getNextMenu() override;
};



#endif //MENU_H
