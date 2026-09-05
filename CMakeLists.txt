cmake_minimum_required(VERSION 3.16)

project(Pomeowdoro LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(Pomeowdoro
    src/main.cpp
    src/createaccount.cpp
    src/createaccount.h
    src/pomoclock.cpp
    src/pomoclock.h
    src/resources.qrc
)

target_link_libraries(Pomeowdoro PRIVATE Qt6::Widgets)

add_executable(PomeowdoroLogin
    src/login.cpp
)

target_link_libraries(PomeowdoroLogin PRIVATE Qt6::Widgets)