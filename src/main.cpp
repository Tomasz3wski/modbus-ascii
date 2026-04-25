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

    std::vector<uint8_t> text = {'H', 'e', 'l', 'l', 'o', ' ', 'M', 'O', 'D', 'B', 'U', 'S'};
    master.sendTransaction(0x01, 0x01, text, false);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    ParsedFrame response = master.sendTransaction(0x01, 0x02, {}, true);

    std::cout << "valid: " << response.valid << '\n';
    if (response.valid){
        std::string received(response.data.begin(), response.data.end());
        std::cout << "reveived: " << received << '\n';
    }

    slave.stop();
    slaveThread.join();

    return 0;
}