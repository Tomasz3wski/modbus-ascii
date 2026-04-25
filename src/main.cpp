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

    master.sendTransaction(0x01, 0x01, text1, false);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    master.sendTransaction(0x00, 0x01, text2);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    master.sendTransaction(0x02, 0x01, text3, false);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    slave.stop();
    slaveThread.join();

    return 0;
}