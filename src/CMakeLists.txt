cmake_minimum_required(VERSION 3.13.4)

set(CMAKE_PREFIX_PATH "C:/Qt/6.11.1/mingw_64")
project(Pomeowdoro)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

add_executable(${PROJECT_NAME} main.cpp pomoclock.cpp pomoclock.h resources.qrc createaccount.cpp createaccount.h)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Sql)