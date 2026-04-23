#include "Slave.hpp"
#include "../serial/SerialPort.hpp"

Slave::Slave(SerialPort& port) : mAddress(1), interCharTimeout(100), mPort(port) {}

ParsedFrame Slave::receiveRequest(){
    std::string rawResult = "";
    State state = State::IDLE;

    while(true){
        int timeout = (state == State::IDLE) ? 100000 : interCharTimeout;
        std::string ch = mPort.read(1, timeout);

        if (state == State::IDLE){
            if (ch.empty()) continue;
            if (ch.back() == ':'){
                state = State::RECEIVING;
                rawResult += ch;
            }
        }
        else if (state == State::RECEIVING){
            if (ch.empty()) break;
            rawResult += ch;
            if (ch.back() == '\n') break;
        }
    }
    return parseFrame(rawResult);
}

void Slave::setAddress(uint8_t address){
    if (address == 0) mAddress = 1;
    else if (address > 247) mAddress = 247;
    else mAddress = address;
}

void Slave::setInterCharTimeout(int ms){
    if (ms <= 0) interCharTimeout = 0;
    else if (ms > 1000) interCharTimeout = 1000;
    else interCharTimeout = (ms / 10) * 10;
}

uint8_t Slave::getAddress() const{
    return mAddress;
}

int Slave::getInterCharTimeout() const{
    return interCharTimeout;
}
