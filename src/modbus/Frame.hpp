#pragma once

#include <string>
#include <vector>

struct ParsedFrame {
    uint8_t address;
    uint8_t function;
    std::vector<uint8_t> data;
    bool valid;
};

ParsedFrame parseFrame(const std::string& raw);

std::string buildFrame(uint8_t address, uint8_t function, const std::vector<uint8_t> &data);