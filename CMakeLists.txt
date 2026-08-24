cmake_minimum_required(VERSION 3.16)
project(PomodoroClock LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(PomodoroClock
    SRC/main.cpp
    SRC/Pomeowdoro.cpp
    SRC/Pomeowdoro.h
)

target_link_libraries(PomodoroClock PRIVATE Qt6::Widgets)