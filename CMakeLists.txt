cmake_minimum_required(VERSION 3.10)

project(Turing VERSION 1.0.0 LANGUAGES CXX)

SET(PROJECT_SOURCE_DIR ./turing-project)
SET(SRC_LIST ${PROJECT_SOURCE_DIR}/main.cpp ${PROJECT_SOURCE_DIR}/parser.hpp ${PROJECT_SOURCE_DIR}/turing_helper.hpp ${PROJECT_SOURCE_DIR}/turing_core.hpp ${PROJECT_SOURCE_DIR}/utils.hpp)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_executable(turing ${SRC_LIST})
