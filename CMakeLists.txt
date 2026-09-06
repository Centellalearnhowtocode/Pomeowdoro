cmake_minimum_required(VERSION 3.16)
project(Pomeowdoro LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/pomoclock.cpp
    src/pomoclock.h
    src/notespage.cpp
    src/notespage.h
    src/createaccount.cpp
    src/createaccount.h
    src/resources.qrc
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Sql)
