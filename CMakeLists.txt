cmake_minimum_required(VERSION 3.10)
project(SdlApp VERSION 1.0)

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra")

# # windows
# # Set Compiler flags
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -Wl,-subsystem,windows") 
# # Set linker flags
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lmingw32")

# add_definitions(-DUNICODE -D_UNICODE)

include_directories(include)
add_executable(sdlapp main.cpp)

add_subdirectory(vendor/SDL)
add_subdirectory(vendor/SDL_image)

if(!WIN32)
    target_link_libraries(sdlapp SDL2 SDL2_image)
else()
    target_link_libraries(
        sdlapp SDL2 SDL2main SDL2_image
    )
endif()