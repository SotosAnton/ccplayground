cmake_minimum_required (VERSION 2.6)


if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


project (Tutorial)

set(SOURCES
  src/test.cpp
)

set(HEADERS
    include/array_2d.hpp
    include/maze.hpp
    include/utils.hpp
    include/stack_builder.hpp
    include/reccursive_builder.hpp
    include/multi_path_builder.hpp
    include/graph.hpp
    include/dijkstra.hpp
)



add_executable(test ${SOURCES} ${HEADERS})
set_property(TARGET test PROPERTY CXX_STANDARD 20)

target_include_directories(test PRIVATE include)
