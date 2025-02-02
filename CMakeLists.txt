cmake_minimum_required(VERSION 3.9)
project(
  3d_engine_cpp
  VERSION 1.0.0
  LANGUAGES CXX)

# Set C++ standard and features
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# GLFW variables
set(GLFW_BUILD_DOCS
    OFF
    CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS
    OFF
    CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES
    OFF
    CACHE BOOL "" FORCE)

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/lib/**/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/lib/**/*.mm")

add_library(LibsModule ${SOURCE_FILES})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/include/glfw EXCLUDE_FROM_ALL)
target_link_libraries(LibsModule PUBLIC glfw)

find_package(OpenGL REQUIRED)
target_link_libraries(LibsModule PUBLIC OpenGL::GL)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/include/imgui)
target_link_libraries(LibsModule PUBLIC imgui)

add_executable(${PROJECT_NAME} main.cpp)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_link_libraries(${PROJECT_NAME} PUBLIC LibsModule)

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/include>)
