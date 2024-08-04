cmake_minimum_required(VERSION 3.1)

project(Mahjong)

# 二进制文件输出路径
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC_FILE)
add_executable(Mahjong ${SRC_FILE})


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

