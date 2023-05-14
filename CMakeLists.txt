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

add_executable(r32 src/main.cpp
        src/serialport/serialportcom.cpp
        src/serialport/serialportcom.h
        src/serialport/handledata.h
        src/serialport/handledata.cpp
        src/serialport/constant.h
        src/widget/delayedbutton.cpp
        src/widget/delayedbutton.h
        src/widget/operatewidget.cpp
        src/widget/operatewidget.h
        src/widget/historydatawidget.cpp
        src/widget/historydatawidget.h
        src/widget/mainwidget.cpp
        src/widget/mainwidget.h
        src/widget/tabwidget.cpp
        src/widget/tabwidget.h
        src/widget/labelbutton.cpp
        src/widget/labelbutton.h
        src/widget/serialportsettingswidget.h
        src/widget/serialportsettingswidget.cpp
        src/widget/operatebasewidget.h
        src/widget/operatebasewidget.cpp
        src/widget/recvr32datawidget.h
        src/widget/recvr32datawidget.cpp
        src/widget/sendcmdwidget.cpp src/widget/statuswidget.h src/widget/statuswidget.cpp)

target_link_libraries(r32
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::SerialPort
        )
