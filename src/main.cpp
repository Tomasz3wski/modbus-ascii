#include "modbus/Frame.hpp"

#include <iostream>
#include <iomanip>

auto print = [](const ParsedFrame& f) {
    std::cout << "valid:    " << f.valid << '\n';
    if (f.valid) {
        std::cout << "address:  " << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<int>(f.address) << '\n';
        std::cout << "function: " << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<int>(f.function) << '\n';
        std::cout << "data:     ";
        for (const uint8_t& b : f.data)
            std::cout << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<int>(b) << ' ';
        std::cout << '\n';
    }
    std::cout << '\n';
};

int main(){
    std::cout << "modbus\n";

    print(parseFrame(":0101000AF4\r\n"));
    print(parseFrame(":0101000AF5\r\n"));
    print(parseFrame("0101000AF4\r\n"));
    print(parseFrame(""));                  

    return 0;
}