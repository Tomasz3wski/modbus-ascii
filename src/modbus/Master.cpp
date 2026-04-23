#include "Master.hpp"
#include "../serial/SerialPort.hpp"
#include "Frame.hpp"

#include <vector>
#include <string>

Master::Master(SerialPort& port) : transactionTimeout(1000), retryCount(3), interCharTimeout(100), mPort(port) {}

bool Master::sendFrame(uint8_t address, uint8_t function, const std::vector<uint8_t>& data){
    if (!mPort.isOpen()) return false;

    std::string frame = buildFrame(address, function, data);
    return mPort.write(frame);
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