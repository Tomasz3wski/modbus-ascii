#include "ui/UI.hpp"
#include "modbus/Master.hpp"

#include <iostream>

int main(){
    Mode mode = selectMode();
    
    if (mode == Mode::MASTER){
        SerialPort port;
        port.open("/dev/tty.usbserial-XXX", 9600);
        Master master(port);
        runMaster(master);
    } else {
        std::cout << "Slave mode\n";
    }

    return 0;
}