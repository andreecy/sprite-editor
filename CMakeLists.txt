cmake_minimum_required(VERSION 3.10)
project(SdlApp VERSION 1.0)

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra")

include_directories(include)
add_executable(sdlapp main.cpp)

add_subdirectory(vendor/SDL2)
add_subdirectory(vendor/SDL_Image)

target_link_libraries(sdlapp SDL2 SDL2_image)
