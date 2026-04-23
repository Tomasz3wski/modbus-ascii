#include "modbus/Frame.hpp"
#include "serial/SerialPort.hpp"
#include "modbus/Master.hpp"
#include "modbus/Slave.hpp"

#include <iostream>
#include <thread>
#include <chrono>

int main(){
    SerialPort masterPort;
    SerialPort slavePort;

    masterPort.open("/dev/ttys002", 9600);
    slavePort.open("/dev/ttys003", 9600);

    if (!masterPort.isOpen() || !slavePort.isOpen()){
        std::cerr << "error\n";
        return 1;
    }

    Slave slave(slavePort);

    std::thread sender([&](){
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        masterPort.write(":0101000AF4\r\n");
    });

    std::cout << "Slave wait...\n";
    ParsedFrame request = slave.receiveRequest();

    std::cout << "valid:    " << request.valid << '\n';
    if (request.valid){
        std::cout << "address:  " << std::hex << static_cast<int>(request.address) << '\n';
        std::cout << "function: " << std::hex << static_cast<int>(request.function) << '\n';
    }

    sender.join();

    return 0;
}