#include "modbus/Frame.hpp"
#include "serial/SerialPort.hpp"
#include "modbus/Master.hpp"
#include "modbus/Slave.hpp"

#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>

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
    Slave slave(slavePort);
    slave.setAddress(0x01);

    std::thread slaveThread([&](){
        ParsedFrame req = slave.receiveRequest();
        if (req.valid) slave.processRequest(req);
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::vector<uint8_t> text = {'H', 'e', 'l', 'l', 'o'};
    master.sendFrame(0x01, 0x01, text);

    slaveThread.join();

    return 0;
}