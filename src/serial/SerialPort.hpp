#pragma once

#include <string>

class SerialPort {
public:
    SerialPort();
    ~SerialPort();

    bool open(const std::string& portName, int baudRate);
    void close();
    bool isOpen() const;

private:
    int fd;
};