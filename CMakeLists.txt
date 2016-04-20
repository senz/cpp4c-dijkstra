cmake_minimum_required(VERSION 3.5)
project(Dijkstras)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fsanitize=address -O -g")

set(SOURCE_FILES
    main.cpp)

add_executable(Dijkstras ${SOURCE_FILES})