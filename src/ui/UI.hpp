#pragma once

#include "../modbus/Master.hpp"

#include <vector>
#include <string>

enum class Mode { MASTER, SLAVE };

struct MasterState {
    uint8_t address = 0x01;
    uint8_t function = 0x01;
    std::string text = "";
    std::vector<std::string> log;
};

Mode selectMode();
void runMaster(Master& master);