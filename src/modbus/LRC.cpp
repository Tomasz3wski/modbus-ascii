#include "LRC.hpp"

#include <vector>

uint8_t computeLRC(const std::vector<uint8_t>& data){
    uint8_t sum = 0x00;
    for(const uint8_t& byte : data){
        sum += byte;
    }

    return ((~sum + 1) & 0xFF);
}