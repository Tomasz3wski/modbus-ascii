#pragma once

#include <string>
#include <vector>

std::string buildFrame(uint8_t address, uint8_t function, const std::vector<uint8_t> &data);