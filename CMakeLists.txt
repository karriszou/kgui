cmake_minimum_required(VERSION 3.16)

set(NAME "gui")

project(${NAME})

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wl,-O2 -std=c++11")

file(GLOB_RECURSE SOURCES ./src/*.c ./src/*.cpp)

add_executable(${NAME}
  ${SOURCES}
)

set(DEPS_DIR "${PROJECT_SOURCE_DIR}/deps/")

target_include_directories(${NAME} PUBLIC
  ${DEPS_DIR}
)

target_link_directories(${NAME} PUBLIC
  ${DEPS_DIR}
)

target_link_libraries(${NAME} PUBLIC
#  glfw3
  glfw
  dl
  pthread
)
