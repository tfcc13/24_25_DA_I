//
// Created by tiago on 26/02/2025.
//

#ifndef MENU_H
#define MENU_H

#define CLEAR "\033[2J\033[1;1H"

#include "RouteNetwork.h"
#include "InputHandler.h"
class RouteNetwork;

class Menu {
protected:
    RouteNetwork& route_network_;
    Menu * invalidInput();

public:

    explicit Menu(RouteNetwork &route_network);

    virtual ~Menu() = default;

    virtual void show() = 0;

    virtual Menu *getNextMenu() = 0;
};

class DataMenu : public Menu {
public:
    DataMenu(RouteNetwork &route_network);
    void show() override;
    Menu *getNextMenu() override;
};

class MainMenu : public Menu {
public:
    MainMenu(RouteNetwork &route_network);
    void show() override;
    Menu *getNextMenu() override;
};

class LocationMenu : public Menu {
public:
    LocationMenu(RouteNetwork &route_network);
    void show() override;
    Menu *getNextMenu() override;

};

class InputMenu : public Menu {
    public:
    InputMenu(RouteNetwork &route_network);
    void show() override;
    Menu *getNextMenu() override;
};

#endif //MENU_H
