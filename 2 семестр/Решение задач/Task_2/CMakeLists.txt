cmake_minimum_required(VERSION 3.20)

project(main_project) 

set(CMAKE_C_COMPILER gcc)

add_executable(tetris main.c tetris.c)

target_include_directories(tetris PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)