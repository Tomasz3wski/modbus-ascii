#include "Master.hpp"
#include "../serial/SerialPort.hpp"
#include "Frame.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <chrono>

Master::Master(SerialPort& port) : transactionTimeout(1000), retryCount(3), interCharTimeout(100), mPort(port) {}

bool Master::sendFrame(uint8_t address, uint8_t function, const std::vector<uint8_t>& data){
    if (!mPort.isOpen()) return false;

    std::string frame = buildFrame(address, function, data);
    return mPort.write(frame);
}

ParsedFrame Master::receiveResponse(){
    std::string rawResult = "";
    auto transactionStart = std::chrono::steady_clock::now();
    
    while(true){
        auto elapsed = std::chrono::steady_clock::now() - transactionStart;
        int elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
        if (elapsedMs > transactionTimeout) break;

        std::string ch = mPort.read(1, interCharTimeout);
        if (ch.empty()) break;

        rawResult += ch;
        if (ch.back() == '\n') break;
    }
    return parseFrame(rawResult);
}

ParsedFrame Master::sendTransaction(uint8_t address, uint8_t function, const std::vector<uint8_t>& data){
    ParsedFrame response;
    response.valid = false;

    for (int i = 0; i <= getRetryCount(); i++){
        sendFrame(address, function, data);

        if (address == 0x00){
            response.valid = true;
            return response;
        }
        response = receiveResponse();

        if (response.valid){
            return response;
        }

        if (i < retryCount){
            std::cerr << "Transaction attempt " << i+1 << '/' << retryCount+1 << " failed, retrying..\n";
        } else {
            std::cerr << "Transaction failed after " << retryCount+1 << " attempts.\n";
        }
    }
    return response;
}

void Master::setTransactionTimeout(int ms){
    if (ms <= 0) transactionTimeout = 0;
    else if (ms > 10000) transactionTimeout = 10000;
    else transactionTimeout = (ms / 100) * 100;
}

void Master::setRetryCount(int count){
    if (count <= 0) retryCount = 0;
    else if (count > 5) retryCount = 5;
    else retryCount = count;
}

void Master::setInterCharTimeout(int ms){
    if (ms <= 0) interCharTimeout = 0;
    else if (ms > 1000) interCharTimeout = 1000;
    else interCharTimeout = (ms / 10) * 10;
}

int Master::getTransactionTimeout() const{
    return transactionTimeout;
}

int Master::getRetryCount() const{
    return retryCount;
}

int Master::getInterCharTimeout() const{
    return interCharTimeout;
}