cmake_minimum_required(VERSION 3.10)
project(MySDL2Project)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")


# Find SDL2 package
find_package(SDL2 REQUIRED)
# find_package(SDL_image REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

# Include SDL2 headers
include_directories(include ${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIR} ${OPENGL_INCLUDE_DIRS})

# Add the executable
add_executable(MySDL2Project opengl_hello.cpp vboindexer.cpp)


# Print SDL2 and SDL2_image information (for debugging)
message("SDL2_INCLUDE_DIRS: ${SDL2_INCLUDE_DIRS}")
message("SDL2_LIBRARIES: ${SDL2_LIBRARIES}")
message("SDL2_IMAGE_INCLUDE_DIRS: ${SDL2_IMAGE_INCLUDE_DIRS}")
message("SDL2_IMAGE_LIBRARIES: ${SDL2_IMAGE_LIBRARIES}")

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${SDL2_IMAGE_LIBRARIES})
