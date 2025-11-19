cmake_minimum_required(VERSION 4.0)
project(cpp_arcade)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG master
)
FetchContent_MakeAvailable(raylib)

add_executable(cpp_arcade main.cpp
        src/tictactoe/TicTacToe.h
        src/tictactoe/TicTacToe.cpp
)

target_link_libraries(cpp_arcade raylib)