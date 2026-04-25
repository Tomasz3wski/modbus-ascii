#include "UI.hpp"
#include "../modbus/Master.hpp"
#include "../modbus/Slave.hpp"

#include <ncurses.h>
#include <sstream>
#include <iomanip>
#include <thread>
#include <mutex>
#include <chrono>

Config selectConfig(){
    echo();
    
    printw("\n=== MODBUS ASCII Terminal ===\n\n");
    
    printw("Enter port (e.g. /dev/tty.usbserial-110): ");
    refresh();
    char portBuf[64];
    getnstr(portBuf, 63);
    
    printw("Enter baud rate (9600/19200/38400/115200): ");
    refresh();
    char baudBuf[8];
    getnstr(baudBuf, 7);
    
    int baud = 9600;
    try {
        baud = std::stoi(baudBuf);
    } catch (...) {
        printw("Invalid baud rate, using 9600\n");
        refresh();
    }
    
    noecho();
    
    Config config;
    config.port = std::string(portBuf);
    config.baudRate = baud;
    return config;
}

Mode selectMode(){
    cbreak();
    noecho();

    printw("MODBUS ASCII Terminal\n");
    printw("---------------------\n");
    printw("1. Master\n");
    printw("2. Slave\n\n");
    printw("Select mode: ");
    refresh();

    while(true){
        int ch = getch();
        if (ch == '1'){
            endwin();
            return Mode::MASTER;
        }
        if (ch == '2'){
            endwin();
            return Mode::SLAVE;
        }

        mvprintw(8, 0, "Invalid key, press 1 or 2");
        refresh();
    }
}

// master

static std::string toHexDump(const std::string& frame){
    std::ostringstream oss;
    for (unsigned char c : frame)
        oss << std::hex << std::uppercase
            << std::setw(2) << std::setfill('0')
            << static_cast<int>(c) << ' ';
    return oss.str();
}

static void drawMaster(const MasterState& state){
    clear();
    printw("=== MASTER MODE ===\n\n");
    printw("Address:  %02X\n", state.address);
    printw("Function: %s\n", state.function == 0x01 ? "1 - Write Text" : "2 - Read Text");
    printw("Text:     %s\n\n", state.text.c_str());
    printw("Timeout:    %d ms\n", state.transactionTimeout);
    printw("Retries:    %d\n", state.retryCount);
    printw("Inter-char: %d ms\n\n", state.interCharTimeout);
    printw("[a] Address  [f] Function  [t] Text\n");
    printw("[o] Timeout  [r] Retries   [i] Inter-char\n");
    printw("[s] Send     [q] Quit\n");
    printw("\n--- Log ---\n");

    int start = std::max(0, (int)state.log.size() - 5);
    for (int i = start; i < (int)state.log.size(); i++){
        printw("%s\n", state.log[i].c_str());
    }
    refresh();
}

void runMaster(Master& master){
    MasterState state;

    while(true){
        drawMaster(state);
        int ch = getch();

        if (ch == 'q') break;

        if (ch == 'a'){
            printw("\nEnter address (hex): ");
            refresh();
            echo();
            char buf[4];
            getnstr(buf, 3);
            noecho();

            try {
                state.address = static_cast<uint8_t>(std::stoi(buf, nullptr, 16));
            } catch (...) {
                state.log.push_back("Invalid address input");
            }
        }

        if (ch == 'f'){
            state.function = (state.function == 0x01) ? 0x02 : 0x01;
        }

        if (ch == 't'){
            printw("\nEnter text: ");
            refresh();
            echo();
            char buf[256];
            getnstr(buf, 255);
            noecho();
            state.text = std::string(buf);
        }

        if (ch == 's'){
            std::vector<uint8_t> data(state.text.begin(), state.text.end());
            std::string frame = buildFrame(state.address, state.function, data);
            state.log.push_back("TX: " + toHexDump(frame));

            bool wait = (state.function == 0x02);
            ParsedFrame response = master.sendTransaction(
                state.address, state.function, data, wait
            );

            if (wait && response.valid){
                std::string rxFrame = buildFrame(
                    response.address, response.function, response.data
                );
                state.log.push_back("RX: " + toHexDump(rxFrame));
                std::string received(response.data.begin(), response.data.end());
                state.log.push_back("RX text: " + received);
            } else if (wait && !response.valid){
                state.log.push_back("RX: timeout / no response");
            }
        }

        if (ch == 'o'){
            printw("\nEnter transaction timeout (0-10000 ms): ");
            refresh();
            echo();
            char buf[8];
            getnstr(buf, 7);
            noecho();
            try {
                state.transactionTimeout = std::stoi(buf);
                master.setTransactionTimeout(state.transactionTimeout);
                state.transactionTimeout = master.getTransactionTimeout();
            } catch (...) {
                state.log.push_back("Invalid timeout input");
            }
        }

        if (ch == 'r'){
            printw("\nEnter retry count (0-5): ");
            refresh();
            echo();
            char buf[4];
            getnstr(buf, 3);
            noecho();
            try {
                master.setRetryCount(std::stoi(buf));
                state.retryCount = master.getRetryCount();
            } catch (...) {
                state.log.push_back("Invalid retry count input");
            }
        }

        if (ch == 'i'){
            printw("\nEnter inter-char timeout (0-1000 ms): ");
            refresh();
            echo();
            char buf[8];
            getnstr(buf, 7);
            noecho();
            try {
                master.setInterCharTimeout(std::stoi(buf));
                state.interCharTimeout = master.getInterCharTimeout();
            } catch (...) {
                state.log.push_back("Invalid inter-char timeout input");
            }
        }
    }
    
    endwin();
}

// slave

void runSlave(Slave& slave){
    echo();
    printw("\n=== SLAVE CONFIGURATION ===\n\n");
    
    printw("Enter address (1-247): ");
    refresh();
    char addrBuf[4];
    getnstr(addrBuf, 3);
    try {
        slave.setAddress(static_cast<uint8_t>(std::stoi(addrBuf)));
    } catch (...) {
        printw("Invalid address, using default (1)\n");
    }
    noecho();

    std::string receivedText = "";
    std::mutex textMutex;

    slave.setOnTextReceived([&](const std::string& text){
        std::lock_guard<std::mutex> lock(textMutex);
        receivedText = text;
    });

    std::thread slaveThread([&](){ slave.run(); });

    nodelay(stdscr, TRUE);

    while(true){
        clear();
        printw("=== SLAVE MODE ===\n\n");
        printw("Address:  %02X\n", slave.getAddress());
        printw("Status:   Waiting for request...\n\n");
        printw("--- Received Text ---\n");

        {
            std::lock_guard<std::mutex> lock(textMutex);
            printw("%s\n", receivedText.c_str());
        }

        printw("\n[q] Quit\n");
        refresh();

        int ch = getch();
        if (ch == 'q'){
            slave.stop();
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    slaveThread.join();
    endwin();
}