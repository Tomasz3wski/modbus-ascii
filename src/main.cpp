#include "ui/UI.hpp"
#include "modbus/Master.hpp"
#include "modbus/Slave.hpp"

#include <iostream>
#include <ncurses.h>

int main(){
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    Config config = selectConfig();
    Mode mode = selectMode();

    SerialPort serialPort;
    serialPort.open(config.port, config.baudRate);

    if (!serialPort.isOpen()){
        endwin();
        std::cerr << "Cannot open port: " << config.port << '\n';
        return 1;
    }

    if (mode == Mode::MASTER){
        Master master(serialPort);
        runMaster(master);
    } else {
        Slave slave(serialPort);
        runSlave(slave);
    }

    return 0;
}