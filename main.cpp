#include <iostream>
#include <stack>

#include "Menu.h"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {
    RouteNetwork *network = new RouteNetwork();
    std::stack<Menu*> menu;


    menu.push(new DataMenu(*network));

    while(!menu.empty()) {
        menu.top()->show();
        Menu * menuPtr = menu.top()->getNextMenu();

        if(menuPtr) {
            if(menuPtr != menu.top()) {
                menu.push(menuPtr);
            }
        }
        else {
            delete menu.top();
            menu.pop();
        }
    }

    std::cout << "BYE! See you next time." << std::endl;
}

// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.