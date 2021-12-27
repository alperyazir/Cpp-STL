cmake_minimum_required(VERSION 3.0.0)
project(Algorithms VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SRC CONFIGURE_DEPENDS "*.h" "*.cpp" "*.hpp")
message("${SRC}")

add_executable(algorithms ${SRC})

