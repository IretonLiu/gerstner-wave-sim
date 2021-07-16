cmake_minimum_required(VERSION 3.17)
project(WaterShader)


set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -g -lGL -lglfw -lGLEW -lGLU")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(SRCS 
	src/main.cpp
	src/gl_helper.cpp
	src/geometry.cpp
	src/camera.cpp
	src/trackball_ctrl.cpp
	src/utilities.cpp)

add_executable(WaterShader ${SRCS})