cmake_minimum_required(VERSION 3.9)
project(
  3d_engine_cpp
  VERSION 1.0.0
  LANGUAGES CXX)

# Set C++ standard and features
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(OPERATING_SYSTEM "Apple")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/include/SDL EXCLUDE_FROM_ALL)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/include/imgui EXCLUDE_FROM_ALL)

# file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/lib/**/*.cpp

# add_library(LibsModule ${SOURCE_FILES}) target_link_libraries(LibsModule
# PUBLIC SDL2::SDL2)

add_executable(${PROJECT_NAME} main.cpp)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/include>)
