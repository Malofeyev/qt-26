cmake_minimum_required(VERSION 3.5)

project(10 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt5 COMPONENTS Widgets Network REQUIRED)


set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        tcpclient.h
        tcpclient.cpp
        structs.h
)

add_executable(10
    ${PROJECT_SOURCES}
)


target_link_libraries(10 PRIVATE Qt5::Widgets Qt5::Network)

