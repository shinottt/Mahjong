cmake_minimum_required(VERSION 3.15)

project(Mahjong-shinohayu VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)


# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

set(SDLTTF_VENDORED ON)

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(third_party/SDL EXCLUDE_FROM_ALL)
add_subdirectory(third_party/SDL_image EXCLUDE_FROM_ALL)
add_subdirectory(third_party/SDL_ttf EXCLUDE_FROM_ALL)

# harfbuzz库的编译问题
target_compile_options(harfbuzz PRIVATE
    "$<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CXX_COMPILER_ID:GNU>>:-Wa,-mbig-obj>"
)

set(CPP_FILES
    src/main.cpp
    src/tile.cpp
    src/enum.cpp
    src/player.cpp
    src/state.cpp
    src/game.cpp
    src/object.cpp
    src/loadAssets/asset_store.cpp
    src/loadAssets/language.cpp
    src/center_server.cpp
)

add_executable(${PROJECT_NAME} ${CPP_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/src/
    ${CMAKE_SOURCE_DIR}/third_party/ 
    ${CMAKE_SOURCE_DIR}/third_party/asio/include/
)

target_link_directories(${PROJECT_NAME} PUBLIC 
)

# Link to the actual SDL3 library.
target_link_libraries(${PROJECT_NAME} PUBLIC
    SDL3_image::SDL3_image   
    SDL3_ttf::SDL3_ttf 
    SDL3::SDL3
)

# 链接windows下的socket库
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC ws2_32)
endif()
