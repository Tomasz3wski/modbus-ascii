cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(modbus-ascii)

find_package(Curses REQUIRED)

add_executable(modbus-ascii
    src/main.cpp
    src/modbus/LRC.cpp
    src/modbus/Frame.cpp
    src/modbus/Master.cpp
    src/modbus/Slave.cpp
    src/serial/SerialPort.cpp
    src/ui/UI.cpp
    )   

target_link_libraries(modbus-ascii ${CURSES_LIBRARIES})
target_include_directories(modbus-ascii PRIVATE ${CURSES_INCLUDE_DIR})