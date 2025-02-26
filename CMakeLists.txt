cmake_minimum_required(VERSION 3.30)
project(DA_PROJECT_1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -pedantic")
set (CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -Werror -Wall -pedantic")

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/data_structures)

add_executable(DA_PROJECT_1 main.cpp
        include/InputHandler.h
        src/InputHandler.cpp
        include/RouteNetwork.h
        src/RouteNetwork.cpp
        src/Location.cpp
        include/Location.h
        include/Route.h
        src/Route.cpp)
