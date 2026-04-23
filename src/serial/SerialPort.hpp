#pragma once

#include <string>

class SerialPort {
public:
    SerialPort();
    ~SerialPort();

    bool open(const std::string& portName, int baudRate);
    void close();
    bool isOpen() const;

    bool write(const std::string& data);
    std::string read(size_t maxBytes, int timeoutMs);

private:
    int fd;
};