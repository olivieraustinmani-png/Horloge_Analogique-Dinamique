cmake_minimum_required(VERSION 3.20)
project(Horloge_SDL3)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL3 REQUIRED)

add_executable(Horloge
    main.cpp
    Game.cpp
)

target_include_directories(Horloge PRIVATE .)
target_link_libraries(Horloge PRIVATE SDL3::SDL3)