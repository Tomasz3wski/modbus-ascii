#include "Slave.hpp"
#include "../serial/SerialPort.hpp"

#include <iostream>
#include <functional>

Slave::Slave(SerialPort& port) : mAddress(1), interCharTimeout(100), mPort(port), isRunning(false) {}

void Slave::setOnTextReceived(std::function<void(const std::string&)> callback){
    onTextReceived = callback;
}

ParsedFrame Slave::receiveRequest(){
    std::string rawResult = "";
    State state = State::IDLE;

    while(true){
        if (!isRunning) return ParsedFrame{0, 0, {}, false};

        int timeout = (state == State::IDLE) ? 500 : interCharTimeout;
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

void Slave::processRequest(const ParsedFrame& frame){
    if (frame.address != mAddress && frame.address != 0x00) return;

    if (frame.function == 0x01){
        storedText.assign(frame.data.begin(), frame.data.end());
        if (onTextReceived) onTextReceived(storedText);
    }
    else if (frame.function == 0x02){
        if (frame.address == 0x00) return;

        std::vector<uint8_t> responseData(storedText.begin(), storedText.end());
        std::string response = buildFrame(mAddress, 0x02, responseData);
        mPort.write(response);
    }
    else{
        std::cerr << "Invalid function - " << frame.function << '\n';
    }
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

void Slave::run(){
    isRunning = true;
    while (isRunning){
        ParsedFrame receivedFrame = receiveRequest();
        if (!isRunning) break;
        if (receivedFrame.valid) processRequest(receivedFrame);
        else std::cerr << "Invalid frame\n";
    }
}

void Slave::stop(){
    isRunning = false;
}

