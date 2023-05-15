cmake_minimum_required(VERSION 3.24)
project(r32)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if (WIN32)
    # 根据自己的地址，如果没配置系统环境
    set(CMAKE_PREFIX_PATH D:/softeware/Qt/5.15.2/mingw81_64)
endif ()

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
        src/widget/sendcmdwidget.cpp
        src/widget/statuswidget.h
        src/widget/statuswidget.cpp)

target_link_libraries(r32
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::SerialPort
        )
