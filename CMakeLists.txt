cmake_minimum_required(VERSION 3.10)

project(ConsoleGames)

set(CMAKE_CXX_STANDARD 20)

find_package(Curses REQUIRED)

include_directories(
    include/
    ${CURSES_INCLUDE_DIR}
)

# add libraray
add_library(Games 
src/games.cpp)

add_executable(MyGame src/main.cpp)

target_link_libraries(MyGame Games ${CURSES_LIBRARIES})
