cmake_minimum_required(VERSION 3.1)

project(Mahjong)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC_FILE)

add_executable(Mahjong ${SRC_FILE})


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)


# 好像没起作用
add_compile_options(Mahjong PUBLIC -W -o test.exe -Og)