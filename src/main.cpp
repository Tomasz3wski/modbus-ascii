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
    master.setRetryCount(2);
    master.setTransactionTimeout(1000);

    ParsedFrame response = master.sendTransaction(0x01, 0x01, {0x00, 0x0A});

    std::cout << "valid: " << response.valid << '\n';


    return 0;
}