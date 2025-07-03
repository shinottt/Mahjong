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


set(COMMON_CPP_FILE
    src/common/asset_store.cpp
    src/common/enum.cpp
    src/common/game.cpp
    src/common/language.cpp
    src/common/player.cpp
    src/common/state.cpp
    src/common/tile.cpp
)

set(SERVER_CPP_FILE
    src/server/main_server.cpp
    src/server/center_server.cpp
)

set(CLIENT_CPP_FILE
    src/client/main_client.cpp
)


add_executable(Mahjong_server ${COMMON_CPP_FILE} ${SERVER_CPP_FILE})

add_executable(Mahjong-shinohayu ${COMMON_CPP_FILE} ${CLIENT_CPP_FILE})

target_include_directories(Mahjong_server PUBLIC
    ${CMAKE_SOURCE_DIR}/src/client/
    ${CMAKE_SOURCE_DIR}/src/server/
    ${CMAKE_SOURCE_DIR}/src/common/
    ${CMAKE_SOURCE_DIR}/third_party/ 
    ${CMAKE_SOURCE_DIR}/third_party/asio/include/
)

target_include_directories(Mahjong-shinohayu PUBLIC
    ${CMAKE_SOURCE_DIR}/src/client/
    ${CMAKE_SOURCE_DIR}/src/server/
    ${CMAKE_SOURCE_DIR}/src/common/
    ${CMAKE_SOURCE_DIR}/third_party/ 
    ${CMAKE_SOURCE_DIR}/third_party/asio/include/
)



# Link to the actual SDL3 library.
target_link_libraries(Mahjong_server PUBLIC
    SDL3_image::SDL3_image   
    SDL3_ttf::SDL3_ttf 
    SDL3::SDL3
)

target_link_libraries(Mahjong-shinohayu PUBLIC
    SDL3_image::SDL3_image   
    SDL3_ttf::SDL3_ttf 
    SDL3::SDL3
)

if(WIN32)
    target_link_libraries(Mahjong_server PRIVATE ws2_32)
    target_link_libraries(Mahjong-shinohayu PRIVATE ws2_32)
endif()
