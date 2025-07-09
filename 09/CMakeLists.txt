cmake_minimum_required(VERSION 3.5)

project(09 LANGUAGES CXX)

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
        udpworker.h
        udpworker.cpp
)


add_executable(09
  main.cpp
  mainwindow.cpp
  mainwindow.h
  udpworker.cpp
  udpworker.h
  mainwindow.ui
)


target_link_libraries(09 PRIVATE Qt5::Widgets Qt5::Network)


