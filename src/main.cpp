#include "ui/UI.hpp"

#include <iostream>

int main(){
    Mode mode = selectMode();
    
    if (mode == Mode::MASTER){
        std::cout << "Master mode\n";
    } else {
        std::cout << "Slave mode\n";
    }

    return 0;
}