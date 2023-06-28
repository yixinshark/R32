cmake_minimum_required(VERSION 3.24)
project(r32)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#if (WIN32)
#    # 根据自己的地址，如果没配置系统环境
#    set(CMAKE_PREFIX_PATH D:/softeware/Qt/5.15.2/mingw81_64)
#endif ()

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Xml
        Sql
        SerialPort
        AxContainer
        REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/src/dbutil)
include_directories(${PROJECT_SOURCE_DIR}/src/serialport)
include_directories(${PROJECT_SOURCE_DIR}/src/widget)
include_directories(${PROJECT_SOURCE_DIR}/src/dbopt)

aux_source_directory(${PROJECT_SOURCE_DIR}/src/dbutil DBUTILS)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/serialport SERIALPORTS)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/widget WIDGETS)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/dbopt DBOPT)

set(R32_SRCS
        ${DBUTILS}
        ${SERIALPORTS}
        ${WIDGETS}
        ${DBOPT}
        src/main.cpp
        )

set(QRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/res.qrc
    )

add_executable(r32
        ${R32_SRCS}
        ${QRCS}
        app_win32.rc
        )

target_link_libraries(r32
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Xml
        Qt5::Sql
        Qt5::SerialPort
        Qt5::AxContainer
        )
