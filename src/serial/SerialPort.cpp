#include "SerialPort.hpp"

#include <string>
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

speed_t toBaudRate(int baudRate) {
    switch(baudRate) {
        case 9600:   return B9600;
        case 19200:  return B19200;
        case 38400:  return B38400;
        case 115200: return B115200;
        default:     return B9600;
    }
}

SerialPort::SerialPort() : fd(-1) {}

SerialPort::~SerialPort() {
    if (isOpen()){
        close();
    }
}

bool SerialPort::open(const std::string& portName, int baudRate){
    fd = ::open(portName.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1){
        std::cerr << "SerialPort: cannot open " << portName << " - " << strerror(errno) << '\n';
        return false;
    }

    struct termios tty;
    tcgetattr(fd, &tty);

    // 8N1 setup
    tty.c_cflag &= ~PARENB; // no odd
    tty.c_cflag &= ~CSTOPB; // 1 stop bit
    tty.c_cflag &= ~CSIZE;  // clear size bits
    tty.c_cflag |= CS8;     // 8 data bits

    tty.c_cflag &= ~CRTSCTS;
    tty.c_oflag &= ~OPOST;

    cfsetispeed(&tty, toBaudRate(baudRate));
    cfsetospeed(&tty, toBaudRate(baudRate));

    tcsetattr(fd, TCSANOW, &tty);
    return true;
}

void SerialPort::close(){
    ::close(fd);
    fd = -1;
}

bool SerialPort::isOpen() const{
    if (fd == -1){
        return false;
    }
    return true;
}