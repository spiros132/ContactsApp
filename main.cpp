#include <iostream>
#include <Contacts.h>
#include <Menu.h>

int main(int, char**){
    Contacts contacts;
    Menu menu(contacts, 25, 10);

    std::cout << "Starting up..." << std::endl;
    
    while(menu.is_running()) {
        menu.update();
    }

    exit(EXIT_SUCCESS);
}
