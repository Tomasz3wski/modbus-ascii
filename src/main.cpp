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
        slave.run();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    std::vector<uint8_t> text1 = {'H', 'e', 'l', 'l', 'o'};
    std::vector<uint8_t> text2 = {'W', 'o', 'r', 'l', 'd'};
    std::vector<uint8_t> text3 = {'M', 'O', 'D', 'B', 'U', 'S'};

    master.sendFrame(0x01, 0x01, text1);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    master.sendFrame(0x01, 0x01, text2);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    master.sendFrame(0x01, 0x01, text3);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    slave.stop();
    slaveThread.join();

    return 0;
}