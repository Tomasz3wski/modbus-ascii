#include "modbus/Frame.hpp"
#include "serial/SerialPort.hpp"
#include "modbus/Master.hpp"

#include <iostream>

int main(){
    SerialPort writer;
    SerialPort reader;

    writer.open("/dev/ttys002", 9600);
    reader.open("/dev/ttys003", 9600);

    if (!writer.isOpen() || !reader.isOpen()){
        std::cerr << "error\n";
        return 1;
    }

    Master master(writer);
    master.sendFrame(0x01, 0x01, {0x00, 0x0A});

    std::string received = reader.read(256, 1000);
    std::cout << "received: " << received << '\n';

    return 0;
}