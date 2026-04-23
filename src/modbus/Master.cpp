#include "Master.hpp"

Master::Master() : transactionTimeout(1000), retryCount(3), interCharTimeout(100) {}

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