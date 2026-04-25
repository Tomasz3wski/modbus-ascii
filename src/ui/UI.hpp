#pragma once

#include "../modbus/Master.hpp"
#include "../modbus/Slave.hpp"

#include <vector>
#include <string>

enum class Mode { MASTER, SLAVE };

struct Config {
    std::string port;
    int baudRate;
};

struct MasterState {
    uint8_t address = 0x01;
    uint8_t function = 0x01;
    std::string text = "";
    std::vector<std::string> log;
    int transactionTimeout = 1000;
    int retryCount = 3;
    int interCharTimeout = 100;
};

Config selectConfig();
Mode selectMode();
void runMaster(Master& master);
void runSlave(Slave& slave);