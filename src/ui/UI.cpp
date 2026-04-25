#include "UI.hpp"
#include "../modbus/Master.hpp"

#include <ncurses.h>
#include <sstream>
#include <iomanip>

Mode selectMode(){
    initscr();
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
    printw("[a] Set address  [f] Set function  [t] Set text\n");
    printw("[s] Send         [q] Quit\n");
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
    }

    endwin();
}