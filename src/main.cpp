#include "modbus/Frame.hpp"

#include <iostream>

int main(){
    std::cout << "modbus\n";

    std::cout << buildFrame(0x01, 0x01, {0x00, 0x0A});

    return 0;
}