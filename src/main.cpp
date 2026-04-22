#include "modbus/Frame.hpp"
#include "serial/SerialPort.hpp"

#include <iostream>
#include <iomanip>

int main(){
    std::cout << "test\n";

    SerialPort port;

    port.open("tty/razdwa", 9600);
    if (port.isOpen()){
        std::cout << "OPEN\n";
    }

    return 0;
}