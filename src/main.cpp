#include "modbus/Frame.hpp"
#include "serial/SerialPort.hpp"
#include "modbus/Master.hpp"

#include <iostream>

int main(){
    SerialPort masterPort;
    SerialPort slavePort;

    masterPort.open("/dev/ttys002", 9600);
    slavePort.open("/dev/ttys003", 9600);

    if (!masterPort.isOpen() || !slavePort.isOpen()){
        std::cerr << "error\n";
        return 1;
    }

    Master master(masterPort);
    master.sendFrame(0x01, 0x01, {0x00, 0x0A});

    std::string request = slavePort.read(256, 1000);
    std::cout << "Slave odebral: " << request << '\n';
    slavePort.write(":0101000AF4\r\n");

    ParsedFrame response = master.receiveResponse();
    std::cout << "valid:    " << response.valid << '\n';
    if (response.valid){
        std::cout << "address:  " << std::hex << static_cast<int>(response.address) << '\n';
        std::cout << "function: " << std::hex << static_cast<int>(response.function) << '\n';
    }

    return 0;
}