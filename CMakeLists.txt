cmake_minimum_required(VERSION 3.5)

project(CommandServer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core Network)

add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/tcpreciver.h
    src/tcpreciver.cpp
)

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Network)
