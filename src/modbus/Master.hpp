#pragma once

class Master {
public:
    Master();

    void setTransactionTimeout(int ms); // 0 - 10000 ms, step 100ms
    void setRetryCount(int count); // 0 - 5
    void setInterCharTimeout(int ms); // 0 - 1000 ms, step 10ms

    int getTransactionTimeout() const;
    int getRetryCount() const;
    int getInterCharTimeout() const;

private:
    int transactionTimeout;
    int retryCount;
    int interCharTimeout;
};