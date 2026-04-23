#include "modbus/Frame.hpp"
#include "serial/SerialPort.hpp"

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

    writer.write("message 123456\n");

    std::string received = reader.read(256, 1000);
    std::cout << "received: " << received << '\n';

    return 0;
}