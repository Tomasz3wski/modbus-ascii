#include "modbus/LRC.hpp"

#include <iostream>

int main(){
    std::cout << "modbus\n";

    std::cout << std::hex << std::uppercase << static_cast<int>(computeLRC({0x01, 0x01, 0x00, 0x0A})) << std::endl;
    std::cout << std::hex << std::uppercase << static_cast<int>(computeLRC({})) << std::endl;

    return 0;
}