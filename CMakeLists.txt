cmake_minimum_required(VERSION 3.24)
project(r32)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        SerialPort
        REQUIRED)

add_executable(r32 src/main.cpp src/serialport/serialportcom.cpp src/serialport/serialportcom.h src/serialport/handledata.cpp src/serialport/handledata.h src/serialport/constant.h)
target_link_libraries(r32
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::SerialPort
        )
