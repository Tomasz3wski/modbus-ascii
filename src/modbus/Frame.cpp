#include "Frame.hpp"
#include "LRC.hpp"

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

ParsedFrame parseFrame(const std::string& raw){
    ParsedFrame parsed;
    if (raw.empty()){
        parsed.valid = false; 
        return parsed; 
    }

    if (raw[0] == ':'){
        std::string temp = raw.substr(1, raw.length() - 3);

        if (temp.length() % 2 == 0){
            std::vector<uint8_t> lrcData;

            for(int i = 0; i < temp.length(); i+=2){
                std::string byteStr = temp.substr(i, 2);
                uint8_t byte = static_cast<uint8_t>(std::stoi(byteStr, nullptr, 16));
                lrcData.push_back(byte);
            }

            uint8_t sum = 0;
            for (const uint8_t& b : lrcData) sum += b;

            if (sum == 0x00){
                parsed.address = lrcData[0];
                parsed.function = lrcData[1];

                lrcData.pop_back();
                lrcData.erase(lrcData.begin(), lrcData.begin() + 2);
                parsed.data = lrcData;

                parsed.valid = true;
                return parsed;
            }
        }
    }
    parsed.valid = false;
    return parsed;
}

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