#include "Frame.hpp"
#include "LRC.hpp"

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

std::string buildFrame(uint8_t address, uint8_t function, const std::vector<uint8_t> &data){
    std::ostringstream oss;
    oss << ':';
    oss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<int>(address);
    oss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<int>(function);

    for (const uint8_t& byte : data){
        oss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<int>(byte);
    }

    std::vector<uint8_t> lrcData = {address, function};
    lrcData.insert(lrcData.end(), data.begin(), data.end());
    oss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<int>(computeLRC(lrcData));

    oss << '\r' << '\n';

    return oss.str();
}